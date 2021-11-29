#pragma once
#include <Common.XY.h>
#include "Game.FishboardCell.h"
namespace game
{
	struct Fishboard
	{
		static const size_t COLUMNS;
		static const size_t ROWS;

		static void Generate(int);

		static void Reveal(int, const common::XY<int>&);

		static FishboardCell Read(int, const common::XY<int>&);

		static bool IsFullyRevealed(int);
		static double ReadProgressPercentage(int);
	};
}
