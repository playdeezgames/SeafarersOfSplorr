#pragma once
#include "Game.Island.District.h"
#include <string>
namespace game::session::island
{
	struct District
	{
		constexpr explicit District(int islandId, const game::island::District& district)
			: islandId(islandId)
			, district(district) {}
		std::string GetName() const;
	private:
		int islandId;
		game::island::District district;
	};
}
