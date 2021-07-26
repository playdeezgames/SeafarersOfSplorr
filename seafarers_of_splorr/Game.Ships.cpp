#include "Game.Ships.h"
#include <map>
namespace game::Ships
{
	const std::map<game::Ship, game::ShipDescriptor> ships =
	{
		{
			game::Ship::RAFT, 
			{
				"raft",
				{
					{game::ship::Statistic::SPEED_FACTOR, 0.75},
					{game::ship::Statistic::TONNAGE, 50.0}
				},
				{
					{game::Commodity::WOOD, 100.0},
					{game::Commodity::LABOR, 50.0}
				}
			}
		},
		{
			game::Ship::JUNK, 
			{
				"junk",
				{
					{game::ship::Statistic::SPEED_FACTOR, 1.0},
					{game::ship::Statistic::TONNAGE, 100.0}
				},
				{
					{game::Commodity::WOOD, 200.0},
					{game::Commodity::LABOR, 75.0}
				}
			}
		}
	};

	static std::list<game::Ship> shipList;

	const game::ShipDescriptor& Read(const game::Ship& ship)
	{
		return ships.find(ship)->second;
	}

	const std::list<game::Ship>& All()
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
}