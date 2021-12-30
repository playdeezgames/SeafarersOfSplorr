#pragma once
#include "Game.Session.World.ShipType.Commodities.h"
#include "Game.ShipType.h"
#include <string>
namespace game::session::world
{
	struct ShipType
	{
		constexpr ShipType(const game::ShipType& shipType) : shipType(shipType) {}
		double GetTotalTonnage() const;//static double GetTotalTonnage(const game::ShipType&);
		std::string GetName() const;//static const std::string& GetName(const game::ShipType&);
		double GetSpeedFactor() const;
		constexpr ship_type::Commodities GetCommodities() const { return ship_type::Commodities(shipType); }//static const std::map<game::Commodity, double> GetCommodities(const game::ShipType&);
	private:
		game::ShipType shipType;
	};
}
