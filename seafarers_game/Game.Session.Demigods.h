#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Demigod.h"
#include <vector>
namespace game::session
{
	struct Demigods
	{
		constexpr Demigods() {}
		void Reset() const;
		void Populate(const Difficulty& difficulty) const;
		std::vector<Demigod> GetAll() const;
		constexpr Demigod GetDemigod(int demigodId) const { return Demigod(demigodId); }
		constexpr void ApplyTurnEffects() const {}
	};
}
