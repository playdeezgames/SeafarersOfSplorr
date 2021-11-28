#pragma once
#include <Common.XY.h>
#include "Game.FishboardCell.h"
namespace game
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
