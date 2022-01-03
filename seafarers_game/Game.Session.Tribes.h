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
		constexpr Tribe GetTribe(int tribeId) const { return Tribe(tribeId); }
		void Reset() const;
		void Populate(const Difficulty& difficulty) const;
	};
}