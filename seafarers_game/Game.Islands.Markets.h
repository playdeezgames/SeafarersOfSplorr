#pragma once
#include <Common.XY.h>
#include "Game.Item.h"
#include "Game.Ship.h"
namespace game::islands
{
	struct Markets
	{
		static void BuyItems(const common::XY<double>&, const game::Item&, size_t);
		static void SellItems(const common::XY<double>&, const game::Item&, size_t);
		static void BuyShip(const common::XY<double>&, const game::ShipType&);//TODO: rename to buyshiptype
		static void SellShip(const common::XY<double>&, const game::ShipType&);//TODO: rename to sellshiptype
	};
}