#include <Common.RNG.h>
#include <Data.Game.FishboardCell.h>
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
			const auto& descriptor = Fishes::Read(fish);
			fishGenerator[fish] = descriptor.junkWeight;
		}
	}

	static void AddFisheries(std::map<Fish, size_t>& fishGenerator)
	{
		auto fisheries = Fisheries::Available();
		for (auto fishery : fisheries)
		{
			fishGenerator[fishery.fish]++;
		}
	}

	static std::map<Fish, size_t> MakeFishGenerator()
	{
		std::map<Fish, size_t> fishGenerator;
		AddFisheries(fishGenerator);
		if (fishGenerator.empty())
		{
			AddJunk(fishGenerator);
		}
		return fishGenerator;
	}

	static Fish GenerateFish()
	{
		std::map<Fish, size_t> fishGenerator = MakeFishGenerator();
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
		auto descriptor = Fishes::Read(fish);
		common::XY<int> origin = GeneratePosition(descriptor.size);
		for (auto& location : descriptor.shape)
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
		auto descriptor = Fishes::Read((Fish)fish);
		game::avatar::Items::Add(descriptor.item, 1);
		auto itemDescriptor = Items::Read(descriptor.item);
		avatar::Log::Write({
			game::Colors::GREEN,
			std::format("You reel in a {}!", itemDescriptor.name) });
	}

	static void ReelInAllFish()
	{
		auto allFish = data::game::FishboardCell::ReadFish();
		for (auto fish : allFish)
		{
			ReelInFish((Fish)fish);
		}
	}

	static void DoRevealCell(const common::XY<int>& location)
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

	void Fishboard::RevealCell(const common::XY<int>& location)
	{
		if (!IsFullyRevealed())
		{
			DoRevealCell(location);
		}
	}

	std::optional<Fishboard> Fishboard::ReadCell(const common::XY<int>& location)
	{
		auto boardCell = data::game::FishboardCell::Read(location);
		if (boardCell)
		{
			std::optional<Fish> fish = std::nullopt;
			if (boardCell.value().fishType.has_value())
			{
				fish = (Fish)boardCell.value().fishType.value();
			}
			Fishboard result =
			{
				boardCell.value().revealed,
				fish
			};
			return result;
		}
		return std::nullopt;
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
		data::game::FishGame::WriteGuesses(data::game::FishGame::ReadGuesses() + 5);
	}
}