#pragma once
#include <Common.XY.h>
#include "Game.Fish.h"
#include "Game.FishboardState.h"
#include <optional>
namespace game
{
	struct Fishboard
	{
		bool revealed;
		std::optional<Fish> fish;

		static const size_t COLUMNS;
		static const size_t ROWS;

		static void Generate();

		static void RevealCell(const common::XY<int>&);
		static bool IsFullyRevealed();

		static Fishboard ReadCell(const common::XY<int>&);

		static int ReadGuesses();
		static bool HasGuessesLeft();

		static double ReadProgressPercentage();

		static bool HasGivenUp();
		static void GiveUp();

		static void AddBait();

		static FishboardState GetState();
	};
}
