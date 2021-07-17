#include "Game.Commodities.h"
#include <map>
namespace game::Commodities
{
	const std::map<game::Commodity, game::CommodityDescriptor> commodityTable =
	{
		{game::Commodity::WHEAT, { "wheat", 1.0, 0.01, 0.01, 0.5}}
	};

	const game::CommodityDescriptor& Read(const game::Commodity& commodity)
	{
		return commodityTable.find(commodity)->second;
	}
}
