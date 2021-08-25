#pragma once
#include <Common.XY.h>
#include "Game.Difficulty.h"
namespace game
{
	struct Merchants
	{
		static void Spawn(const common::XY<double>&);
		static void ApplyTurnEffects();
		static void Reset(const Difficulty&);
	};
}