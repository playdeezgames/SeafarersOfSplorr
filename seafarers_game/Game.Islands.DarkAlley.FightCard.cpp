#include <Common.RNG.h>
#include <Data.Game.Island.DarkAlley.h>
#include <Data.Game.Island.DarkAlley.FightCard.h>
#include "Game.Characters.Statistics.h"
#include "Game.Islands.DarkAlley.FightCard.h"
#include <list>
#include <set>
namespace game::islands::dark_alley
{
	static const size_t CARD_ROWS = 3;
	static const size_t CARD_COLUMNS = 4;
	static const size_t CARD_COUNT = CARD_ROWS * CARD_COLUMNS;

	static const std::list<cards::Rank> FACE_CARD_RANKS =
	{
		cards::Rank::JACK,
		cards::Rank::QUEEN,
		cards::Rank::KING
	};

	static bool IsFaceCard(const cards::Card& card)
	{
		auto rank = std::get<0>(card);
		return std::find(FACE_CARD_RANKS.begin(), FACE_CARD_RANKS.end(), rank) != FACE_CARD_RANKS.end();
	}

	static const std::list<cards::Rank> NONFACE_CARD_RANKS =
	{
		cards::Rank::ACE,
		cards::Rank::DEUCE,
		cards::Rank::THREE,
		cards::Rank::FOUR,
		cards::Rank::FIVE,
		cards::Rank::SIX,
		cards::Rank::SEVEN,
		cards::Rank::EIGHT,
		cards::Rank::NINE,
		cards::Rank::TEN
	};

	static const std::list<cards::Suit> CARD_SUITS =
	{
		cards::Suit::CLUB,
		cards::Suit::DIAMOND,
		cards::Suit::HEART,
		cards::Suit::SPADE
	};

	static const std::map<size_t, std::list<size_t>> ADJACENCIES =
	{
		{0, {1,4,5}},
		{1, {0,2,4,5,6}},
		{2, {1,3,5,6,7}},
		{3, {2,6,7}},
		{4, {0,1,5,8,9}},
		{5, {0,1,2,4,6,8,9,10}},
		{6, {1,2,3,5,7,9,10,11}},
		{7, {2,3,6,10,11}},
		{8, {4,5,9}},
		{9, {4,5,6,8,10}},
		{10, {5,6,7,9,11}},
		{11, {6,7,10}}
	};

	static void WriteCards(
		int islandId, 
		const std::map<size_t, FightCard>& fightCards)
	{
		for (auto& fightCard : fightCards)
		{
			data::game::island::dark_alley::FightCard::Write(islandId, fightCard.first,
				{
					(int)std::get<0>(fightCard.second.card),
					(int)std::get<1>(fightCard.second.card),
					fightCard.second.adjacentSuccesses,
					fightCard.second.revealed
				});
		}
	}

	static size_t DetermineFaceCardCount(int characterId, const data::game::island::DarkAlley& darkAlley)
	{
		auto brawling = game::characters::statistics::Brawling::Current(characterId).value();
		size_t faceCardCount =
			(size_t)(brawling / (brawling + darkAlley.ruffianBrawlingStrength) * (double)CARD_COUNT);
		return
			(faceCardCount < 1) ? (1) :
			(faceCardCount > CARD_COUNT - 1) ? (CARD_COUNT - 1) :
			(faceCardCount);
	}

	static cards::Card GenerateCard(const std::list<cards::Rank>& ranks)
	{
		auto rank = common::RNG::FromList(ranks).value();
		auto suit = common::RNG::FromList(CARD_SUITS).value();
		return std::make_tuple(rank, suit);
	}

	static cards::Card GenerateFaceCard()
	{
		return GenerateCard(FACE_CARD_RANKS);
	}

	static void PlaceCard(
		std::map<size_t, FightCard>& fightCards, 
		cards::Card card)
	{
		size_t index;
		do
		{
			index = common::RNG::FromRange(0u, CARD_COUNT);
		} while (fightCards.find(index) != fightCards.end());
		fightCards[index] =
		{
			card,
			0,
			false
		};
	}

	static void PlaceFaceCards(
		int characterId,
		std::map<size_t, FightCard>& fightCards, 
		std::set<cards::Card>& cards, 
		const data::game::island::DarkAlley& darkAlley)
	{
		size_t faceCardCount = DetermineFaceCardCount(characterId, darkAlley);
		while (faceCardCount > 0)
		{
			auto card = GenerateFaceCard();
			if (!cards.contains(card))
			{
				cards.insert(card);
				PlaceCard(fightCards, card);
				faceCardCount--;
			}
		}
	}

	static cards::Card GenerateNonfaceCard()
	{
		return GenerateCard(NONFACE_CARD_RANKS);
	}

	static cards::Card PlaceNonfaceCard(std::set<cards::Card>& cards)
	{
		auto card = GenerateNonfaceCard();
		while (cards.contains(card))
		{
			card = GenerateNonfaceCard();
		}
		cards.insert(card);
		return card;
	}

	static int CountAjacencies(std::map<size_t, FightCard>& fightCards, int index)
	{
		auto adjacencies = ADJACENCIES.find(index)->second;
		int count = 0;
		for (auto adjacency : adjacencies)
		{
			auto neighbor = fightCards.find(adjacency);
			if (neighbor != fightCards.end() && IsFaceCard(neighbor->second.card))
			{
				count++;
			}
		}
		return count;
	}

	static void PlaceNonfaceCards(std::map<size_t, FightCard>& fightCards, std::set<cards::Card>& cards)
	{
		for (size_t index = 0; index < CARD_COUNT; ++index)
		{
			if (fightCards.find(index) == fightCards.end())
			{
				auto card = PlaceNonfaceCard(cards);

				int count = CountAjacencies(fightCards, (int)index);
				fightCards[index] =
				{
					card,
					count,
					false
				};
			}
		}
	}

	static void DoGenerate(int characterId, int islandId, const data::game::island::DarkAlley& darkAlley)
	{
		std::map<size_t, FightCard> fightCards;
		std::set<cards::Card> cards;
		PlaceFaceCards(characterId, fightCards, cards, darkAlley);
		PlaceNonfaceCards(fightCards, cards);
		WriteCards(islandId, fightCards);
	}

	void FightCard::Generate(int characterId, int islandId)
	{
		data::game::island::dark_alley::FightCard::Clear(islandId);
		auto darkAlley = data::game::island::DarkAlley::Read(islandId);
		if (darkAlley)
		{
			DoGenerate(characterId, islandId, darkAlley.value());
		}
	}

	static FightCard DataToFightCard(const data::game::island::dark_alley::FightCard& data)
	{
		auto card = std::make_tuple((cards::Rank)data.rank, (cards::Suit)data.suit);
		return 
		{
			card,
			data.adjacent,
			data.shown,
			IsFaceCard(card)
		};
	}

	std::map<size_t, FightCard> FightCard::Read(int islandId)
	{
		std::map<size_t, FightCard> result;
		auto entries = data::game::island::dark_alley::FightCard::Read(islandId);
		for (auto& entry : entries)
		{
			result[entry.first] = DataToFightCard(entry.second);
		}
		return result;
	}

	std::optional<FightCard> FightCard::Pick(int islandId, size_t index)
	{
		auto data = data::game::island::dark_alley::FightCard::Read(islandId, index);
		if (data)
		{
			auto cardData = data.value();
			if (cardData.shown)
			{
				return std::nullopt;
			}
			cardData.shown = true;
			data::game::island::dark_alley::FightCard::Write(islandId, index, cardData);
			return DataToFightCard(cardData);
		}
		return std::nullopt;
	}
}