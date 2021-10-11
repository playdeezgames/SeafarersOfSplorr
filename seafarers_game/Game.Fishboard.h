#pragma once
#include <Common.XY.h>
#include "Game.Fish.h"
#include "Game.FishboardCell.h"
#include "Game.FishboardState.h"
#include <optional>
namespace game//20211011
{
	struct Fishboard
	{

		static const size_t COLUMNS;
		static const size_t ROWS;

		static void Generate();

		static void RevealCell(const common::XY<int>&);//TODO: call this "reveal"?
		static bool IsFullyRevealed();

		static FishboardCell ReadCell(const common::XY<int>&);//TODO: call this "read"?

		static int ReadGuesses();
		static bool HasGuessesLeft();

		static double ReadProgressPercentage();

		static bool HasGivenUp();
		static void GiveUp();

		static void AddBait();

		static FishboardState GetState();
	};
}
