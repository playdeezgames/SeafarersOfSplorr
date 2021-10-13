#pragma once
#include <Common.XY.h>
#include "Game.Fish.h"
#include "Game.FishboardCell.h"
#include "Game.FishGameState.h"
#include <optional>
namespace game//20211013
{
	struct Fishboard
	{
		static const size_t COLUMNS;
		static const size_t ROWS;

		static void Generate();

		static void Reveal(const common::XY<int>&);

		static FishboardCell Read(const common::XY<int>&);

		static bool IsFullyRevealed();
		static double ReadProgressPercentage();
	};
}
