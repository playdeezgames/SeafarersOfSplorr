#pragma once
#include "Game.Difficulty.h"
namespace game::session
{
	struct Demigods
	{
		void Reset(const Difficulty&) const;
		void ApplyTurnEffects() const;
	};
}
