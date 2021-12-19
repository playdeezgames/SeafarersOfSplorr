#pragma once
#include "Game.Commodity.h"
namespace game::session::island
{
	struct Market
	{
		constexpr Market(int islandId, const game::Commodity& commodity) : islandId(islandId), commodity(commodity) {}
	private:
		int islandId;
		game::Commodity commodity;
	};
}
