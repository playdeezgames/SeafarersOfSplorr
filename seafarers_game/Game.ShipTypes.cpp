#include <Common.RNG.h>
#include <Common.Utility.List.h>
#include <Common.Utility.Table.h>
#include "Game.BerthType.h"
#include "Game.ShipTypes.h"
#include "Game.Ship.Property.h"
namespace game
{
	struct ShipDescriptor
	{
		std::string name;
		std::map<game::ship::Property, double> properties;
		std::map<game::Commodity, double> commodities;
		size_t initialShipGenerationWeight;
		std::map<BerthType, size_t> berths;
	};

	static const std::map<game::ShipType, ShipDescriptor> ships =
	{
		{
			game::ShipType::RAFT,
			{
				"raft",
				{
					{game::ship::Property::SPEED_FACTOR, 1.0},
					{game::ship::Property::TONNAGE, 50.0}
				},
				{
					{game::Commodity::WOOD, 100.0},
					{game::Commodity::LABOR, 50.0}
				},
				1,
				{
					{BerthType::CAPTAIN, 1}
				}
			}
		},
		{
			game::ShipType::JUNK,
			{
				"junk",
				{
					{game::ship::Property::SPEED_FACTOR, 1.25},
					{game::ship::Property::TONNAGE, 100.0}
				},
				{
					{game::Commodity::WOOD, 200.0},
					{game::Commodity::LABOR, 75.0}
				},
				0,
				{
					{BerthType::CAPTAIN, 1}
				}
			}
		}
	};

	static std::list<game::ShipType> shipList;

	static const ShipDescriptor& Read(const game::ShipType& ship)
	{
		return ships.find(ship)->second;
	}

	static const std::map<ShipType, ShipDescriptor>& AllDescriptors()
	{
		return ships;
	}

	std::map<game::ShipType, size_t> initialShipGenerator =
		common::utility::Table::Accumulate<ShipType, ShipDescriptor, std::map<ShipType, size_t>>(
			AllDescriptors,
			[](std::map<ShipType, size_t>& result, const ShipType& shipType, const ShipDescriptor& descriptor)
			{
				result[shipType] = descriptor.initialShipGenerationWeight;
			});

	static double ReadProperty(const ShipType& shipType, const ship::Property& property)
	{
		return Read(shipType).properties.find(property)->second;
	}

	double ShipTypes::GetSpeedFactor(const game::ShipType& shipType)
	{
		return ReadProperty(shipType, ship::Property::SPEED_FACTOR);
	}

	const std::map<game::Commodity, double> ShipTypes::GetCommodities(const game::ShipType& shipType)
	{
		return Read(shipType).commodities;
	}
}