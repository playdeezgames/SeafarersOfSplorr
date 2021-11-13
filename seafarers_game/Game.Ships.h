#pragma once
#include "Game.Difficulty.h"
namespace game
{
	struct Ships
	{
		static void Reset(const game::Difficulty&);
		static void ApplyTurnEffects();
	};
}
