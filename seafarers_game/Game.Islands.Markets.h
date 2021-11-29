#pragma once
#include "Game.Item.h"
#include "Game.ShipType.h"
namespace game::islands
{
	struct Markets
	{
		static void BuyItems(int, const game::Item&, size_t);
		static void SellItems(int, const game::Item&, size_t);
		static void BuyShipType(int, const game::ShipType&);
		static void SellShipType(int, const game::ShipType&);
	};
}