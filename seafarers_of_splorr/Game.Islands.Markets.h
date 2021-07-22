#pragma once
#include "Common.XY.h"
#include "Game.Item.h"
namespace game::islands::Markets
{
	void BuyItems(const common::XY<double>&, const game::Item&, size_t);
	void SellItems(const common::XY<double>&, const game::Item&, size_t);
}