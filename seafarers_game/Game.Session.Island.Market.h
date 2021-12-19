#pragma once
#include "Game.Commodity.h"
namespace game::session::island
{
	struct Market
	{
		constexpr Market(int islandId, const game::Commodity& commodity) : islandId(islandId), commodity(commodity) {}
		//TODO: purchase value of an item subtype
		//TODO: sale value of an item subtype
	private:
		int islandId;
		game::Commodity commodity;
	};
}
