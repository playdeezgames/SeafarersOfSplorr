#pragma once
#include "Game.Difficulty.h"
#include <optional>
namespace game
{
	struct Session
	{
		constexpr Session() {}
		void ApplyTurnEffects() const;
		void Reset() const;
		void Populate(const Difficulty& difficulty) const;
	};
}
