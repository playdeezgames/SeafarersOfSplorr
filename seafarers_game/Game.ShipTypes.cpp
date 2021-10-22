#include <Common.RNG.h>
#include <Common.Utility.h>
#include <Common.Utility.Table.h>
#include "Game.BerthType.h"
#include "Game.ShipTypes.h"
#include "Game.Ship.Property.h"
#include <map>
#include <string>
namespace game//20211017
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
		return common::Utility::ListFromTable(shipList, ships);
	}


	static const std::map<ShipType, ShipDescriptor>& AllDescriptors()
	{
		return ships;
	}

	std::map<game::ShipType, size_t> initialShipGenerator =
		common::utility::Table::AccumulateTable<ShipType, ShipDescriptor, std::map<ShipType, size_t>>(
			AllDescriptors,
			[](std::map<ShipType, size_t>& result, const ShipType& shipType, const ShipDescriptor& descriptor)
			{
				result[shipType] = descriptor.initialShipGenerationWeight;
			});

	game::ShipType ShipTypes::GenerateForAvatar()
	{
		return common::RNG::FromGenerator(initialShipGenerator);
	}

	static double ReadProperty(const ShipType& shipType, const ship::Property& property)
	{
		return Read(shipType).properties.find(property)->second;
	}

	double ShipTypes::GetTotalTonnage(const game::ShipType& shipType)
	{
		return ReadProperty(shipType, ship::Property::TONNAGE);
	}

	const std::string& ShipTypes::GetName(const game::ShipType& shipType)
	{
		return Read(shipType).name;
	}

	double ShipTypes::GetSpeedFactor(const game::ShipType& shipType)
	{
		return ReadProperty(shipType, ship::Property::SPEED_FACTOR);
	}

	const std::map<game::Commodity, double> ShipTypes::GetCommodities(const game::ShipType& shipType)
	{
		return Read(shipType).commodities;
	}

	static std::map<ShipType, std::list<ShipStatistic>> statisticLists;

	static const std::list<ShipStatistic>& LoadDefaultStatistics(const ShipType& shipType)
	{
		return statisticLists[shipType] = common::utility::Table::AccumulateTable<ShipStatistic, ShipStatisticDescriptor, std::list<ShipStatistic>>(
			[shipType]() { return Read(shipType).statistics;  },
			[](std::list<ShipStatistic>& result, const ShipStatistic& statistic, const ShipStatisticDescriptor&) 
			{ result.push_back(statistic); });
	}

	std::list<ShipStatistic> ShipTypes::GetStatistics(const game::ShipType& shipType)
	{
		return 
			common::utility::Table::TryGetKey(statisticLists, shipType)
				.value_or(LoadDefaultStatistics(shipType));
	}

	static const ShipStatisticDescriptor& ReadStatistic(const ShipType& shipType, const ShipStatistic& statistic)
	{
		return Read(shipType).statistics.find(statistic)->second;
	}

	std::optional<double> ShipTypes::GetMinimumStatistic(const game::ShipType& shipType, const game::ShipStatistic& statistic)
	{
		return ReadStatistic(shipType, statistic).minimum;
	}

	std::optional<double> ShipTypes::GetMaximumStatistic(const game::ShipType& shipType, const game::ShipStatistic& statistic)
	{
		return ReadStatistic(shipType, statistic).maximum;
	}

	double ShipTypes::GetInitialStatistic(const game::ShipType& shipType, const game::ShipStatistic& statistic)
	{
		return ReadStatistic(shipType, statistic).initial;
	}
}