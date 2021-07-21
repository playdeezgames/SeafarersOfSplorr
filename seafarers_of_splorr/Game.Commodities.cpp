#include "Game.Commodities.h"
#include <map>
namespace game::Commodities
{
	const std::map<game::Commodity, game::CommodityDescriptor> commodityTable =
	{
		{game::Commodity::WHEAT, { "wheat", 1.0, 0.01, 0.01, 0.15}},
		{game::Commodity::WOOD, { "wood", 7.5, 0.01, 0.01, 0.15}},
		{game::Commodity::WOOL, { "wool", 2.5, 0.01, 0.01, 0.15}},
		{game::Commodity::LABOR, { "labor", 1.0, 0.01, 0.01, 0.15}},
		{game::Commodity::MEAT, { "meat", 5.0, 0.01, 0.01, 0.15}},
		{game::Commodity::JOOLS, { "jools", 100.0, 0.01, 0.01, 0.15}}
	};

	const std::list<game::Commodity> commodityList =
	{
		game::Commodity::WHEAT
	};

	const game::CommodityDescriptor& Read(const game::Commodity& commodity)
	{
		return commodityTable.find(commodity)->second;
	}

	const std::list<game::Commodity>& All()
	{
		return commodityList;
	}
}
