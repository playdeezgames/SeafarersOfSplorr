#pragma once
#include "Game.Session.Island.Market.h"
namespace game::session::island
{
	struct Markets
	{
		constexpr Markets(int islandId) : islandId(islandId) {}
		constexpr Market GetMarket(const game::Commodity& commodity) const { return Market(islandId, commodity); }
	private:
		int islandId;
	};
}
