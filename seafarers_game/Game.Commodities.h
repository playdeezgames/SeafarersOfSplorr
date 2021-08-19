#pragma once
#include "Game.Commodity.h"
#include "Game.CommodityDescriptor.h"
#include <list>
namespace game::Commodities
{
	const game::CommodityDescriptor& Read(const game::Commodity&);
	const std::list<game::Commodity>& All();
}
