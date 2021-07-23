#include "Game.Ships.h"
#include <map>
namespace game::Ships
{
	const std::map<game::Ship, game::ShipDescriptor> ships =
	{
		{game::Ship::RAFT, {"raft"}},
		{game::Ship::JUNK, {"junk"}}
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