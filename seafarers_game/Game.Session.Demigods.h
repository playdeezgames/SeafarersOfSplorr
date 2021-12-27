#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Demigod.h"
#include <list>
namespace game::session
{
	struct Demigods
	{
		void Reset() const;
		void Populate(const Difficulty&) const;
		void ApplyTurnEffects() const;
		std::list<Demigod> GetAll() const;
	};
}
