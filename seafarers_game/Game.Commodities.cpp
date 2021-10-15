#include <Common.Utility.h>
#include "Game.Commodities.h"
#include <map>
#include <string>
namespace game//20211015
{
	struct CommodityDescriptor
	{
		std::string name;
		double basePrice;
		double supplyFactor;
		double demandFactor;
		double discount;
	};

	static const std::map<game::Commodity, game::CommodityDescriptor> commodityTable =
	{
		{
			game::Commodity::WHEAT, 
			{ 
				"wheat", 
				1.0, 
				0.05, 
				0.05, 
				0.15
			}
		},
		{
			game::Commodity::WOOD, 
			{ 
				"wood", 
				7.5, 
				0.05, 
				0.05, 
				0.15
			}
		},
		{
			game::Commodity::WOOL, 
			{ 
				"wool", 
				2.5, 
				0.05, 
				0.05, 
				0.15
			}
		},
		{
			game::Commodity::LABOR, 
			{ 
				"labor", 
				1.0, 
				0.05, 
				0.05, 
				0.15
			}
		},
		{
			game::Commodity::MEAT, 
			{
				"meat", 
				5.0, 
				0.05, 
				0.05, 
				0.15
			}
		},
		{
			game::Commodity::JOOLS, 
			{ 
				"jools", 
				100.0, 
				0.05, 
				0.05, 
				0.15
			}
		},
		{
			game::Commodity::SILK, 
			{ 
				"silk", 
				50.0, 
				0.05, 
				0.05, 
				0.15
			}
		},
		{
			game::Commodity::SEAWEED, 
			{ 
				"seaweed", 
				0.01, 
				0.00, 
				0.00, 
				0.15
			}
		},
		{
			game::Commodity::SALT, 
			{ 
				"salt", 
				1.0, 
				0.02, 
				0.02, 
				0.15
			}
		}
	};

	static std::list<game::Commodity> commodityList;

	static const game::CommodityDescriptor& Read(const game::Commodity& commodity)
	{
		return commodityTable.find(commodity)->second;
	}

	const std::list<game::Commodity>& Commodities::All()
	{
		return common::Utility::ListFromTable(commodityList, commodityTable);
	}

	double Commodities::GetBasePrice(const game::Commodity& commodity)
	{
		return Read(commodity).basePrice;
	}

	double Commodities::GetDemandFactor(const game::Commodity& commodity)
	{
		return Read(commodity).demandFactor;
	}

	double Commodities::GetSupplyFactor(const game::Commodity& commodity)
	{
		return Read(commodity).supplyFactor;
	}

	double Commodities::GetDiscount(const game::Commodity& commodity)
	{
		return Read(commodity).discount;
	}
}
