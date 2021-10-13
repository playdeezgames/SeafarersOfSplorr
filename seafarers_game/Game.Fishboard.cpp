#include <Common.RNG.h>
#include <Data.Game.FishboardCell.h>
#include <Data.Game.Fishery.h>
#include <Data.Game.FishGame.h>
#include "Game.Avatar.Items.h"
#include "Game.Avatar.Log.h"
#include "Game.Colors.h"
#include "Game.Fishboard.h"
#include "Game.Fisheries.h"
#include "Game.Fishes.h"
#include "Game.Items.h"
#include <format>
#include <map>
namespace game
{
	const size_t Fishboard::COLUMNS = 5;//static members!
	const size_t Fishboard::ROWS = 5;//static members!

	static const int INITIAL_GUESSES = 5;
	static const int ADDITIONAL_GUESSES = 5;
	static const Fish DEFAULT_FISH = Fish::COD;//TODO: FOR NOW!!
	static const Item BAIT_ITEM = Item::BAIT;

	static void ClearFishGame()
	{
		data::game::FishGame::Clear();
		data::game::FishGame::Start(INITIAL_GUESSES);
	}

	static void ClearFishboard()
	{
		data::game::FishboardCell::Clear();
		for (size_t column = 0; column < Fishboard::COLUMNS; ++column)
		{
			for (size_t row = 0; row < Fishboard::ROWS; ++row)
			{
				data::game::FishboardCell::Write({
					{(int)column,(int)row},
					false,
					std::nullopt });
			}
		}
	}

	static void AddJunk(std::map<Fish, size_t>& fishGenerator)
	{
		for (auto fish : Fishes::AllJunk())
		{
			fishGenerator[fish] = Fishes::GetJunkWeight(fish);
		}
	}

	static bool GenerateFisheryHasStock(const Fishery& fishery)
	{
		std::map<bool, size_t> inStock =
		{
			{true, fishery.stock},
			{false, fishery.depletion}
		};
		return (common::RNG::FromGenerator(inStock, false));
	}

	static void AddFishFromFishery(int fisheryId, const Fishery& fishery, std::map<Fish, size_t>& fishGenerator)
	{
		if (GenerateFisheryHasStock(fishery))
		{
			data::game::FishGame::WriteFisheryId(fisheryId);
			fishGenerator[(Fish)fishery.fish] = 1;
		}
	}

	static void AddFishFromFisheries(const std::map<int, size_t>& fisheryGenerator, std::map<Fish, size_t>& fishGenerator)
	{
		int fisheryId = common::RNG::FromGenerator(fisheryGenerator, 0);
		auto fishery = Fisheries::Read(fisheryId);
		if (fishery)
		{
			AddFishFromFishery(fisheryId, fishery.value(), fishGenerator);
		}
	}

	static std::map<Fish, size_t> AddFisheries(const std::map<int, size_t>& fisheryGenerator)
	{
		std::map<Fish, size_t> fishGenerator;
		if (!fisheryGenerator.empty())
		{
			AddFishFromFisheries(fisheryGenerator, fishGenerator);
		}
		return fishGenerator;
	}

	static std::map<Fish, size_t> MakeFishGenerator(const std::map<int, size_t>& fisheryGenerator)
	{
		std::map<Fish, size_t> fishGenerator = AddFisheries(fisheryGenerator);
		if (fishGenerator.empty())
		{
			AddJunk(fishGenerator);
		}
		return fishGenerator;
	}

	static std::map<int, size_t> MakeFisheryGenerator()
	{
		std::map<int, size_t> generator;
		auto fisheries = Fisheries::Available();
		for (auto fishery : fisheries)
		{
			generator[fishery.fisheryId] = fishery.stock + fishery.depletion;
		}
		return generator;
	}

	static Fish GenerateFish()
	{
		std::map<Fish, size_t> fishGenerator = MakeFishGenerator(MakeFisheryGenerator());
		return common::RNG::FromGenerator(fishGenerator, DEFAULT_FISH);
	}

	static common::XY<int> GeneratePosition(const common::XY<size_t>& size)
	{
		int x = (int)common::RNG::FromRange(0u, Fishboard::COLUMNS + 1u - size.GetX());
		int y = (int)common::RNG::FromRange(0u, Fishboard::ROWS + 1u - size.GetY());
		return { x,y };
	}

	static void PlaceFish(Fish fish)
	{
		common::XY<int> origin = GeneratePosition(Fishes::GetSize(fish));
		for (auto& location : Fishes::GetShape(fish))
		{
			data::game::FishboardCell::Write({
				(location + origin),
				false,
				(int)fish });
		}
	}

	void Fishboard::Generate()
	{
		ClearFishGame();
		ClearFishboard();
		avatar::Items::Remove(BAIT_ITEM, 1);
		PlaceFish(GenerateFish());
	}

	static void MakeGuess(const common::XY<int>& location, int guesses)
	{
		auto boardCell = data::game::FishboardCell::Read(location).value();
		boardCell.revealed = true;
		data::game::FishboardCell::Write(boardCell);
		data::game::FishGame::WriteGuesses(guesses - 1);
	}

	static void ReelInFish(Fish fish)
	{
		auto item = Fishes::GetItem(fish);
		game::avatar::Items::Add(item, 1);
		avatar::Log::Write({
			game::Colors::GREEN,
			std::format("You reel in a {}!", Items::GetName(item)) });
	}

	static void DepleteFishery()
	{
		auto fisheryId = data::game::FishGame::ReadFisheryId();
		if (fisheryId)
		{
			auto fishery = data::game::Fishery::Read(fisheryId.value()).value();
			fishery.depletion++;
			data::game::Fishery::Write(fishery);
		}
	}

	static void ReelInAllFish()
	{
		auto allFish = data::game::FishboardCell::ReadFish();
		for (auto fish : allFish)
		{
			ReelInFish((Fish)fish);
		}
		DepleteFishery();
	}

	static void DoReveal(const common::XY<int>& location)
	{
		auto guesses = Fishboard::ReadGuesses();
		if (guesses > 0)
		{
			MakeGuess(location, guesses);
		}
		if (Fishboard::IsFullyRevealed())
		{
			ReelInAllFish();
		}
	}

	void Fishboard::Reveal(const common::XY<int>& location)
	{
		if (!IsFullyRevealed())
		{
			DoReveal(location);
		}
	}

	FishboardCell Fishboard::Read(const common::XY<int>& location)
	{
		auto boardCell = data::game::FishboardCell::Read(location).value();
		std::optional<Fish> fish =
			(boardCell.fishType.has_value()) ?
			(std::optional<Fish>((Fish)boardCell.fishType.value())) :
			(std::nullopt);
		return 
		{
			boardCell.revealed,
			fish
		};
	}

	int Fishboard::ReadGuesses()
	{
		return data::game::FishGame::ReadGuesses();
	}

	double Fishboard::ReadProgressPercentage()
	{
		return 100.0 * data::game::FishboardCell::ReadRevealedFishCount() / data::game::FishboardCell::ReadFishCount();
	}

	bool Fishboard::IsFullyRevealed()
	{
		return data::game::FishboardCell::ReadRevealedFishCount() == data::game::FishboardCell::ReadFishCount();
	}

	bool Fishboard::HasGivenUp()
	{
		return data::game::FishGame::ReadGivenUp();
	}

	void Fishboard::GiveUp()
	{
		data::game::FishGame::WriteGuesses(0);
		data::game::FishGame::WriteGivenUp(true);
	}

	bool Fishboard::HasGuessesLeft()
	{
		return ReadGuesses() > 0;
	}

	void Fishboard::AddBait()
	{
		avatar::Items::Remove(Item::BAIT, 1);
		data::game::FishGame::WriteGuesses(data::game::FishGame::ReadGuesses() + ADDITIONAL_GUESSES);
	}

	FishGameState Fishboard::GetState()
	{
		return 
			(IsFullyRevealed()) ? (FishGameState::DONE) :
			(HasGivenUp()) ? (FishGameState::GAVE_UP) :
			(ReadGuesses() > 0) ? (FishGameState::FISHING) :
			(FishGameState::OUT_OF_GUESSES);
	}

}