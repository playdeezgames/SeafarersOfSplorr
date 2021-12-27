#pragma once
#include "Game.Difficulty.h"
namespace game::session
{
	struct Demigods
	{
		void Reset() const;
		void Populate(const Difficulty&) const;
		void ApplyTurnEffects() const;
	};
}
