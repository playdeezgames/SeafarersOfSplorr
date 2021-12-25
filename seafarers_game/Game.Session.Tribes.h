#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Tribe.h"
#include <list>
namespace game::session
{
	struct Tribes
	{
		constexpr Tribes() {}
		std::list<Tribe> GetAll() const;
		Tribe GetTribe(int) const;
		void Reset(const Difficulty&) const;
	};
}