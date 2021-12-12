#include "Game.Islands.h"
#include "Game.Ship.h"
#include "Game.Ship.Crew.h"
#include "Game.Session.Ship.h"
namespace game::session
{
	Ship::Ship(int shipId)
		: shipId(shipId)
	{
		
	}

	std::list<ship::Berth> Ship::GetBerths() const
	{
		std::list<ship::Berth> result;
		auto berths = game::ship::Crew::Read(shipId);
		for (auto berth : berths)
		{
			result.push_back(ship::Berth(berth.avatarId));
		}
		return result;
	}

	std::optional<double> Ship::GetHeading() const
	{
		return game::Ship::GetHeading(shipId);
	}

	std::optional<double> Ship::GetSpeed() const
	{
		return game::Ship::GetSpeed(shipId);
	}

	static Islands ToIslands(const std::list<game::Island> islands)
	{
		std::list<int> islandIds;
		for (auto island : islands)
		{
			islandIds.push_back(island.id);
		}
		return game::session::Islands(islandIds);
	}

	Islands Ship::GetDockableIslands() const
	{
		return ToIslands(game::Islands::GetDockableIslands(shipId));
	}

	Islands Ship::GetNearbyIslands() const
	{
		return ToIslands(game::Islands::GetViewableIslands(shipId));
	}
}