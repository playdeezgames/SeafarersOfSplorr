#include <Common.RNG.h>
#include "Game.BerthType.h"
#include "Game.ShipTypes.h"
#include "Game.Ship.Property.h"
#include <map>
#include <string>
namespace game
{
	struct ShipDescriptor
	{
		std::string name;
		std::map<game::ship::Property, double> properties;
		std::map<game::Commodity, double> commodities;
		std::map<ShipStatistic, ShipStatisticDescriptor> statistics;
		size_t initialShipGenerationWeight;
		std::map<BerthType, size_t> berths;
	};

	static const std::map<game::ShipType, game::ShipDescriptor> ships =
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
				{
					{
						ShipStatistic::PORT_FOULING,
						{
							0.0,
							0.125,
							0.0
						}
					},
					{
						ShipStatistic::STARBOARD_FOULING,
						{
							0.0,
							0.125,
							0.0
						}
					},
					{
						ShipStatistic::FOULING_RATE,
						{
							0.001,
							0.001,
							0.001
						}
					}
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
				{
					{
						ShipStatistic::PORT_FOULING,
						{
							0.0,
							0.125,
							0.0
						}
					},
					{
						ShipStatistic::STARBOARD_FOULING,
						{
							0.0,
							0.125,
							0.0
						}
					},
					{
						ShipStatistic::FOULING_RATE,
						{
							0.001,
							0.001,
							0.001
						}
					}
				},
				0,
				{
					{BerthType::CAPTAIN, 1}
				}
			}
		}
	};

	static std::list<game::ShipType> shipList;

	static const game::ShipDescriptor& Read(const game::ShipType& ship)
	{
		return ships.find(ship)->second;
	}

	const std::list<game::ShipType>& ShipTypes::All()
	{
		if (shipList.empty())
		{
			for (auto& ship : ships)
			{
				shipList.push_back(ship.first);
			}
		}
		return shipList;
	}

	std::map<game::ShipType, size_t> initialShipGenerator;

	game::ShipType ShipTypes::GenerateForAvatar()
	{
		if (initialShipGenerator.empty())
		{
			for (auto ship : All())
			{
				auto descriptor = Read(ship);
				if (descriptor.initialShipGenerationWeight > 0)
				{
					initialShipGenerator[ship] = descriptor.initialShipGenerationWeight;
				}
			}
		}
		return common::RNG::FromGenerator(initialShipGenerator, game::ShipType::RAFT);
	}

	double ShipTypes::GetTotalTonnage(const game::ShipType& ship)
	{
		return Read(ship).properties.find(ship::Property::TONNAGE)->second;
	}

	const std::string& ShipTypes::GetName(const game::ShipType& ship)
	{
		return Read(ship).name;
	}

	double ShipTypes::GetSpeedFactor(const game::ShipType& ship)
	{
		return Read(ship).properties.find(ship::Property::SPEED_FACTOR)->second;
	}

	const std::map<game::Commodity, double> ShipTypes::GetCommodities(const game::ShipType& ship)
	{
		return Read(ship).commodities;
	}

	const std::map<ShipStatistic, ShipStatisticDescriptor>& ShipTypes::GetStatistics(const game::ShipType& ship)
	{
		return Read(ship).statistics;
	}
}