#include "Game.Fisheries.h"
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

	std::optional<common::XY<double>> Ship::GetLocation() const
	{
		return game::Ship::GetLocation(shipId);
	}

	game::session::Fisheries Ship::GetFisheries() const
	{
		auto fisheries = game::Fisheries::Available(shipId);
		std::list<int> fisheryIds;
		for (auto fishery : fisheries)
		{
			fisheryIds.push_back(fishery.fisheryId);
		}
		return game::session::Fisheries(fisheryIds);
	}

	void Ship::SetHeading(double heading) const
	{
		game::Ship::SetHeading(shipId, heading);
	}

	void Ship::SetSpeed(double speed) const
	{
		game::Ship::SetSpeed(shipId, speed);
	}

}