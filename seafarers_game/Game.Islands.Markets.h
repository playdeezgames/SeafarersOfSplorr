#pragma once
#include <Common.XY.h>
#include "Game.Item.h"
#include "Game.Ship.h"
namespace game::islands::Markets
{
	void BuyItems(const common::XY<double>&, const game::Item&, size_t);
	void SellItems(const common::XY<double>&, const game::Item&, size_t);
	void BuyShip(const common::XY<double>&, const game::Ship&);
	void SellShip(const common::XY<double>&, const game::Ship&);
}