#pragma once
#include "Game.Difficulty.h"
namespace game
{
	struct Fisheries
	{
		static void Reset(const Difficulty&);
		static void ApplyTurnEffects();
	};
}