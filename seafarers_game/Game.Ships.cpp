#include <Common.RNG.h>
#include "Game.Ships.h"
#include "Game.Ship.Property.h"
#include <map>
#include <string>
namespace game
{
	static const std::map<std::string, size_t> shipNameAdjectives =
	{
		{"Delicious",1},
		{"Voluptuous",1},
		{"Sturdy",1},
		{"Grumpy",1},
		{"Hard",1},
		{"Slimy",1},
		{"Boaty",1},
		{"Smelly",1},
		{"Focus on",1},
		{"Huge",1},
		{"Flogging",1}
	};

	static const std::map<std::string, size_t> shipNameNouns =
	{
		{"Booty",1},
		{"Deliverance",1},
		{"Tosser",1},
		{"Wumpus",1},
		{"Wood",1},
		{"McBoatface",1},
		{"McGrumpyface",1},
		{"Cat",1},
		{"Happiness",1},
		{"Tracks of Land",1},
		{"Bishop",1}
	};

	struct ShipDescriptor
	{
		std::string name;
		std::map<game::ship::Property, double> properties;
		std::map<game::Commodity, double> commodities;
		std::map<ShipStatistic, ShipStatisticDescriptor> statistics;
		size_t initialShipGenerationWeight;
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
				1
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
				0
			}
		}
	};

	static std::list<game::ShipType> shipList;

	static const game::ShipDescriptor& Read(const game::ShipType& ship)
	{
		return ships.find(ship)->second;
	}

	const std::list<game::ShipType>& Ships::All()
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

	game::ShipType Ships::GenerateForAvatar()
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

	double Ships::GetTotalTonnage(const game::ShipType& ship)
	{
		return Read(ship).properties.find(ship::Property::TONNAGE)->second;
	}

	const std::string& Ships::GetName(const game::ShipType& ship)
	{
		return Read(ship).name;
	}

	double Ships::GetSpeedFactor(const game::ShipType& ship)
	{
		return Read(ship).properties.find(ship::Property::SPEED_FACTOR)->second;
	}

	const std::map<game::Commodity, double> Ships::GetCommodities(const game::ShipType& ship)
	{
		return Read(ship).commodities;
	}

	const std::map<ShipStatistic, ShipStatisticDescriptor>& Ships::GetStatistics(const game::ShipType& ship)
	{
		return Read(ship).statistics;
	}
}