#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Demigod.h"
#include <list>
namespace game::session
{
	struct Demigods
	{
		constexpr Demigods() {}
		void Reset() const;
		void Populate(const Difficulty& difficulty) const;
		std::list<Demigod> GetAll() const;
		constexpr Demigod GetDemigod(int demigodId) const { return Demigod(demigodId); }
		constexpr void ApplyTurnEffects() const {}
	};
}
