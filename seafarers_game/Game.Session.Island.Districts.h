#pragma once
#include "Game.Session.Island.District.h"
#include <vector>
namespace game::session::island
{
	struct Districts
	{
		constexpr explicit Districts(int islandId) : islandId(islandId) {}
		std::vector<District> GetDistricts() const;
	private:
		int islandId;
	};
}
