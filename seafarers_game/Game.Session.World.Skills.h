#pragma once
#include "Game.Difficulty.h"
namespace game::session::world
{
	struct Skills
	{
		constexpr Skills() {}
		void Reset() const;
		void Populate(const Difficulty& difficulty) const;
	};
}
