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
	const size_t Fishboard::COLUMNS = 5;
	const size_t Fishboard::ROWS = 5;

	static int INITIAL_GUESSES = 5;

	static void ClearFishGame()
	{
		data::game::FishGame::Clear();
		data::game::FishGame::WriteGuesses(INITIAL_GUESSES);
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

	static std::optional<Fish> GenerateFish()
	{
		std::map<Fish, size_t> fishGenerator;
		auto fisheries = Fisheries::Available();
		for (auto fishery : fisheries)
		{
			fishGenerator[fishery.fish]++;
		}
		if (fishGenerator.empty())
		{
			for (auto fish : Fishes::All())
			{
				const auto& descriptor = Fishes::Read(fish);
				if (descriptor.junkWeight > 0)
				{
					fishGenerator[fish] = descriptor.junkWeight;
				}
			}
		}
		if (!fishGenerator.empty())
		{
			return common::RNG::FromGenerator(fishGenerator, Fish::COD);//note - default is just there to satisfy the parameter list
		}
		return std::nullopt;
	}

	void Fishboard::Generate()
	{
		ClearFishGame();
		ClearFishboard();
		avatar::Items::Remove(Item::BAIT, 1);
		auto fish = GenerateFish();
		if (fish)
		{
			auto descriptor = Fishes::Read(fish.value());
			int x = (int)common::RNG::FromRange(0u, COLUMNS + 1u - descriptor.size.GetX());
			int y = (int)common::RNG::FromRange(0u, ROWS + 1u - descriptor.size.GetY());
			common::XY<int> origin = { x,y };
			for (auto& location : descriptor.shape)
			{
				data::game::FishboardCell::Write({
					(location + origin),
					false,
					(int)fish.value() });
			}
		}
	}

	void Fishboard::Reveal(const common::XY<int>& location)
	{
		if (!IsRevealed())
		{
			auto guesses = ReadGuesses();
			if (guesses > 0)
			{
				auto boardCell = data::game::FishboardCell::Read(location).value();
				boardCell.revealed = true;
				data::game::FishboardCell::Write(boardCell);
				data::game::FishGame::WriteGuesses(guesses - 1);
			}
			if (IsRevealed())
			{
				Fish fish = (Fish)data::game::FishboardCell::ReadFish().value();
				auto descriptor = Fishes::Read(fish);
				game::avatar::Items::Add(descriptor.item, 1);
				auto itemDescriptor = Items::Read(descriptor.item);
				avatar::Log::Write({
					game::Colors::GREEN,
					std::format("You reel in a {}!", itemDescriptor.name) });
			}
		}
	}

	std::optional<Fishboard> Fishboard::Read(const common::XY<int>& location)
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

	bool Fishboard::IsRevealed()
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