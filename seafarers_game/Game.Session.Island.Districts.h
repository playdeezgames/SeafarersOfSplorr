#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Island.District.h"
#include <vector>
namespace game::session::island
{
	struct Districts
	{
		constexpr explicit Districts(int islandId) : islandId(islandId) {}
		constexpr District GetDistrict(const game::island::District& district) { return District(islandId, district); }
		std::vector<District> GetDistricts() const;
		void Populate(const game::Difficulty& difficulty) const;
	private:
		int islandId;
	};
}
