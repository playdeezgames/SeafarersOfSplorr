#include "Game.Session.Commodity.h"
#include <map>
namespace game::session
{
	struct CommodityDescriptor
	{
		std::string name;
		double basePrice;
		double supplyFactor;
		double demandFactor;
		double discount;
	};

	static const std::map<game::Commodity, CommodityDescriptor> commodityTable =
	{
		{
			game::Commodity::WHEAT,
			{
				"wheat",
				1.0,
				0.05,
				0.05,
				0.85
			}
		},
		{
			game::Commodity::WOOD,
			{
				"wood",
				7.5,
				0.05,
				0.05,
				0.85
			}
		},
		{
			game::Commodity::WOOL,
			{
				"wool",
				2.5,
				0.05,
				0.05,
				0.85
			}
		},
		{
			game::Commodity::LABOR,
			{
				"labor",
				1.0,
				0.05,
				0.05,
				0.85
			}
		},
		{
			game::Commodity::MEAT,
			{
				"meat",
				5.0,
				0.05,
				0.05,
				0.85
			}
		},
		{
			game::Commodity::JOOLS,
			{
				"jools",
				1.0,
				0.0,
				0.0,
				1.0
			}
		},
		{
			game::Commodity::SILK,
			{
				"silk",
				50.0,
				0.05,
				0.05,
				0.85
			}
		},
		{
			game::Commodity::SEAWEED,
			{
				"seaweed",
				0.01,
				0.00,
				0.00,
				0.85
			}
		},
		{
			game::Commodity::SALT,
			{
				"salt",
				1.0,
				0.02,
				0.02,
				0.85
			}
		}
	};

	double Commodity::GetBasePrice() const
	{
		return commodityTable.find(commodity)->second.basePrice;
	}

	double Commodity::GetDemandFactor() const
	{
		return commodityTable.find(commodity)->second.demandFactor;
	}

	double Commodity::GetSupplyFactor() const
	{
		return commodityTable.find(commodity)->second.supplyFactor;
	}

	double Commodity::GetDiscount() const
	{
		return commodityTable.find(commodity)->second.discount;
	}

	const std::string& Commodity::GetName() const
	{
		return commodityTable.find(commodity)->second.name;
	}
}