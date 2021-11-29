#include <Cards.Card.h>
#include <Cards.Deck.h>
#include <Data.Game.Island.h>
#include <Data.Game.Island.DarkAlley.GamblingHand.h>
#include "Game.Islands.DarkAlley.GamblingHand.h"
#include "Game.Islands.Features.h"
namespace game::islands::dark_alley//20211014
{
	static data::game::island::dark_alley::GamblingHand DealBettableHand()
	{
		data::game::island::dark_alley::GamblingHand hand;
		do
		{
			cards::Deck<cards::Card> deck(cards::All());
			deck.Shuffle();
			hand =
				{
					cards::ToInt(deck.Draw().value()),
					cards::ToInt(deck.Draw().value()),
					cards::ToInt(deck.Draw().value())
				};
		} while (std::abs((int)std::get<0>(cards::OfInt(hand.secondCard)) - (int)std::get<0>(cards::OfInt(hand.firstCard))) < 2);
		return hand;
	}

	bool GamblingHand::Deal(const common::XY<double>& location)
	{
		if (!game::islands::Features::Read(data::game::Island::Find(location).value(), game::Feature::DARK_ALLEY))
		{
			return false;
		}
		data::game::island::dark_alley::GamblingHand data =
			DealBettableHand();
		data::game::island::dark_alley::GamblingHand::Write(data::game::Island::Find(location).value(), data);
		return true;
	}

	std::vector<cards::Card> GamblingHand::Read(const common::XY<double>& location)
	{
		std::vector<cards::Card> result;
		auto hand = data::game::island::dark_alley::GamblingHand::Read(data::game::Island::Find(location).value());
		if (hand)
		{
			result.push_back(cards::OfInt(hand.value().firstCard));
			result.push_back(cards::OfInt(hand.value().secondCard));
			result.push_back(cards::OfInt(hand.value().thirdCard));
		}
		return result;
	}

	bool GamblingHand::IsWinner(const common::XY<double>& location)
	{
		auto hand = Read(location);
		if(!hand.empty())
		{
			auto first = std::get<0>(hand[0]);
			auto second = std::get<0>(hand[1]);
			auto third = std::get<0>(hand[2]);
			if (first > second)
			{
				return (third > second) && (first > third);
			}
			else if (second > first)
			{
				return (third > first) && (second > third);
			}
		}
		return false;
	}

}