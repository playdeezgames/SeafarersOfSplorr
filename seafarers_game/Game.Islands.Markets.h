#pragma once
#include "Game.ShipType.h"
namespace game::islands
{
	struct Markets
	{
		static void BuyShipType(int, const game::ShipType&);
		static void SellShipType(int, const game::ShipType&);
	};
}