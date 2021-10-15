#include <Common.RNG.h>
#include "Game.BerthType.h"
#include "Game.ShipTypes.h"
#include "Game.Ship.Property.h"
#include <map>
#include <string>
namespace game
{
	struct ShipStatisticDescriptor
	{
		std::optional<double> minimum;
		std::optional<double> maximum;
		double initial;
	};

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
		return common::RNG::FromGenerator(initialShipGenerator);
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

	static std::map<ShipType, std::list<ShipStatistic>> statisticLists;

	const std::list<ShipStatistic>& ShipTypes::GetStatistics(const game::ShipType& ship)
	{
		if (statisticLists.contains(ship))
		{
			return statisticLists.find(ship)->second;
		}
		std::list<ShipStatistic> statistics;
		for (auto entry : Read(ship).statistics)
		{
			statistics.push_back(entry.first);
		}
		statisticLists[ship] = statistics;
		return statisticLists[ship];
	}

	std::optional<double> ShipTypes::GetMinimumStatistic(const game::ShipType& shipType, const game::ShipStatistic& statistic)
	{
		return Read(shipType).statistics.find(statistic)->second.minimum;
	}

	std::optional<double> ShipTypes::GetMaximumStatistic(const game::ShipType& shipType, const game::ShipStatistic& statistic)
	{
		return Read(shipType).statistics.find(statistic)->second.maximum;

	}

	double ShipTypes::GetInitialStatistic(const game::ShipType& shipType, const game::ShipStatistic& statistic)
	{
		return Read(shipType).statistics.find(statistic)->second.initial;
	}
}