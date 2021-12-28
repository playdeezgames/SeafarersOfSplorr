#include <Common.RNG.h>
#include <Data.Game.FishboardCell.h>
#include <Data.Game.Fishery.h>
#include <Data.Game.FishGame.h>
#include <Data.Game.Character.Ship.h>
#include "Game.Fishboard.h"
#include "Game.Fisheries.h"
#include "Game.Fishes.h"
#include "Game.FishGame.h"
#include "Game.Items.h"
namespace game
{
	const size_t Fishboard::COLUMNS = 5;//static members!
	const size_t Fishboard::ROWS = 5;//static members!

	static const int INITIAL_GUESSES = 5;
	static const Item BAIT_ITEM = Item::BAIT;

	static void ClearFishGame(int characterId)
	{
		data::game::FishGame::Clear(characterId);
		data::game::FishGame::Start(characterId, INITIAL_GUESSES);
	}

	static void ClearFishboard(int characterId)
	{
		data::game::FishboardCell::Clear(characterId);
		for (size_t column = 0; column < Fishboard::COLUMNS; ++column)
		{
			for (size_t row = 0; row < Fishboard::ROWS; ++row)
			{
				data::game::FishboardCell::Write(
					characterId,
					{{(int)column,(int)row},
					false,
					std::nullopt });
			}
		}
	}

	//vvvv Fish/Junk Generator
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
		return (common::RNG::FromGenerator(inStock));
	}

	static void AddFishFromFishery(int characterId, int fisheryId, const Fishery& fishery, std::map<Fish, size_t>& fishGenerator)
	{
		if (GenerateFisheryHasStock(fishery))
		{
			data::game::FishGame::WriteFisheryId(characterId, fisheryId);
			fishGenerator[(Fish)fishery.fish] = 1;
		}
	}

	static void AddFishFromFisheries(int characterId, const std::map<int, size_t>& fisheryGenerator, std::map<Fish, size_t>& fishGenerator)
	{
		int fisheryId = common::RNG::FromGenerator(fisheryGenerator);
		auto fishery = Fisheries::Read(fisheryId);
		if (fishery)
		{
			AddFishFromFishery(characterId, fisheryId, fishery.value(), fishGenerator);
		}
	}

	static std::map<Fish, size_t> AddFisheries(int characterId, const std::map<int, size_t>& fisheryGenerator)
	{
		std::map<Fish, size_t> fishGenerator;
		if (!fisheryGenerator.empty())
		{
			AddFishFromFisheries(characterId, fisheryGenerator, fishGenerator);
		}
		return fishGenerator;
	}

	static std::map<Fish, size_t> MakeFishGenerator(int characterId, const std::map<int, size_t>& fisheryGenerator)
	{
		std::map<Fish, size_t> fishGenerator = AddFisheries(characterId, fisheryGenerator);
		if (fishGenerator.empty())
		{
			AddJunk(fishGenerator);
		}
		return fishGenerator;
	}

	static std::map<int, size_t> MakeFisheryGenerator(int shipId)
	{
		std::map<int, size_t> generator;
		auto fisheries = Fisheries::Available(shipId);
		for (auto fishery : fisheries)
		{
			generator[fishery.fisheryId] = fishery.stock + fishery.depletion;
		}
		return generator;
	}

	static Fish GenerateFish(int characterId)
	{
		int shipId = data::game::character::Ship::ReadShipForCharacter(characterId).value();
		std::map<Fish, size_t> fishGenerator = MakeFishGenerator(characterId, MakeFisheryGenerator(shipId));
		return common::RNG::FromGenerator(fishGenerator);
	}
	//^^^^  Fish/Junk Generator

	static common::XY<int> GeneratePosition(const common::XY<size_t>& size)
	{
		int x = (int)common::RNG::FromRange(0u, Fishboard::COLUMNS + 1u - size.GetX());
		int y = (int)common::RNG::FromRange(0u, Fishboard::ROWS + 1u - size.GetY());
		return { x,y };
	}

	static void PlaceFish(int characterId, Fish fish)
	{
		common::XY<int> origin = GeneratePosition(Fishes::GetSize(fish));
		for (auto& location : Fishes::GetShape(fish))
		{
			data::game::FishboardCell::Write(characterId, {
				(location + origin),
				false,
				(int)fish });
		}
	}

	void Fishboard::Generate(int characterId)
	{
		ClearFishGame(characterId);
		ClearFishboard(characterId);
		PlaceFish(characterId, GenerateFish(characterId));
	}

	static void MakeGuess(int characterId, const common::XY<int>& location, int guesses)
	{
		auto boardCell = data::game::FishboardCell::Read(characterId, location).value();
		boardCell.revealed = true;
		data::game::FishboardCell::Write(characterId, boardCell);
		data::game::FishGame::WriteGuesses(characterId, guesses - 1);
	}

	static void ReelInFish(int characterId, Fish fish)
	{
		auto item = Fishes::GetItem(fish);
	}

	static void DepleteFishery(int characterId)
	{
		auto fisheryId = data::game::FishGame::ReadFisheryId(characterId);
		if (fisheryId)
		{
			auto fishery = data::game::Fishery::Read(fisheryId.value()).value();
			fishery.depletion++;
			data::game::Fishery::Write(fishery);
		}
	}

	static void ReelInAllFish(int characterId)
	{
		auto allFish = data::game::FishboardCell::ReadFish(characterId);
		for (auto fish : allFish)
		{
			ReelInFish(characterId, (Fish)fish);
		}
		DepleteFishery(characterId);
	}

	static void DoReveal(int characterId, const common::XY<int>& location)
	{
		auto guesses = FishGame::ReadGuesses(characterId);
		if (guesses > 0)
		{
			MakeGuess(characterId, location, guesses);
		}
		if (Fishboard::IsFullyRevealed(characterId))
		{
			ReelInAllFish(characterId);
		}
	}

	void Fishboard::Reveal(int characterId, const common::XY<int>& location)
	{
		if (!IsFullyRevealed(characterId))
		{
			DoReveal(characterId, location);
		}
	}

	FishboardCell Fishboard::Read(int characterId, const common::XY<int>& location)
	{
		auto boardCell = data::game::FishboardCell::Read(characterId, location).value();
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

	bool Fishboard::IsFullyRevealed(int characterId)
	{
		return data::game::FishboardCell::ReadRevealedFishCount(characterId) == data::game::FishboardCell::ReadFishCount(characterId);
	}

	double Fishboard::ReadProgressPercentage(int characterId)
	{
		return 100.0 * data::game::FishboardCell::ReadRevealedFishCount(characterId) / data::game::FishboardCell::ReadFishCount(characterId);
	}
}