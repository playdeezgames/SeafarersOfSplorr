#pragma once
#include "Game.Difficulty.h"
#include <optional>
namespace game
{
	struct Session
	{
		Session() = delete;
		static void ApplyTurnEffects();
		static void Reset();
		static void Populate(const Difficulty& difficulty);
	};
}
