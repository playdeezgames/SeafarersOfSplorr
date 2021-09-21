#include <Common.RNG.h>
#include <Data.Game.FishBoard.h>
#include <Data.Game.FishGame.h>
#include "Game.Fishboard.h"
#include "Game.Fisheries.h"
#include "Game.Fishes.h"
#include <map>
namespace game
{
	const size_t Fishboard::COLUMNS = 5;
	const size_t Fishboard::ROWS = 5;

	static int INITIAL_GUESSES = 10;

	static void ClearFishGame()
	{
		data::game::FishGame::Clear();
		data::game::FishGame::WriteGuesses(INITIAL_GUESSES);
	}

	static void ClearFishboard()
	{
		data::game::FishBoard::Clear();
		for (size_t column = 0; column < Fishboard::COLUMNS; ++column)
		{
			for (size_t row = 0; row < Fishboard::ROWS; ++row)
			{
				data::game::FishBoard::Write({
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

		auto fish = GenerateFish();
		if (fish)
		{
			auto descriptor = Fishes::Read(fish.value());
			int x = (int)common::RNG::FromRange(0u, COLUMNS + 1u - descriptor.size.GetX());
			int y = (int)common::RNG::FromRange(0u, ROWS + 1u - descriptor.size.GetY());
			common::XY<int> origin = { x,y };
			for (auto& location : descriptor.shape)
			{
				data::game::FishBoard::Write({
					(location + origin),
					false,
					(int)fish.value() });
			}
		}
	}

	void Fishboard::Reveal(const common::XY<int>& location)
	{
		auto guesses = ReadGuesses();
		if (guesses > 0)
		{
			auto boardCell = data::game::FishBoard::Read(location).value();
			boardCell.revealed = true;
			data::game::FishBoard::Write(boardCell);
			data::game::FishGame::WriteGuesses(guesses - 1);
		}
	}

	std::optional<Fishboard> Fishboard::Read(const common::XY<int>& location)
	{
		auto boardCell = data::game::FishBoard::Read(location);
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
		return 100.0 * data::game::FishBoard::ReadRevealedFishCount() / data::game::FishBoard::ReadFishCount();
	}
}