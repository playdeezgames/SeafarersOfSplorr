#pragma once
#include <Common.XY.h>
#include "Game.Item.h"
#include "Game.ShipType.h"
namespace game::islands
{
	struct Markets
	{
		static void BuyItems(const common::XY<double>&, const game::Item&, size_t);
		static void SellItems(const common::XY<double>&, const game::Item&, size_t);
		static void BuyShipType(const common::XY<double>&, const game::ShipType&);
		static void SellShipType(const common::XY<double>&, const game::ShipType&);
	};
}