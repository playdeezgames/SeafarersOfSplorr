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

	static std::optional<double> TryGetHeading(int shipId)
	{
		return game::Ship::GetHeading(shipId);
	}

	double Ship::GetHeading() const
	{
		return TryGetHeading(shipId).value();
	}

	static std::optional<double> TryGetSpeed(int shipId)
	{
		return game::Ship::GetSpeed(shipId);
	}

	double Ship::GetSpeed() const
	{
		return TryGetSpeed(shipId).value();
	}

	static Islands ToIslands(const std::list<game::Island> islands)
	{
		std::list<int> islandIds;
		for (auto island : islands)
		{
			islandIds.push_back(island.id);
		}
		return game::session::Islands([islandIds]() {return islandIds; });//!!!!
	}

	Islands Ship::GetDockableIslands() const
	{
		return ToIslands(game::Islands::GetDockableIslands(shipId));
	}

	Islands Ship::GetNearbyIslands() const
	{
		return ToIslands(game::Islands::GetViewableIslands(shipId));
	}

	static std::optional<common::XY<double>> TryGetLocation(int shipId)
	{
		return game::Ship::GetLocation(shipId);
	}

	common::XY<double> Ship::GetLocation() const
	{
		return TryGetLocation(shipId).value();
	}

	game::session::Fisheries Ship::GetFisheries() const
	{
		auto fisheries = game::Fisheries::Available(shipId);
		std::list<int> fisheryIds;
		for (auto fishery : fisheries)
		{
			fisheryIds.push_back(fishery.fisheryId);
		}
		return game::session::Fisheries([fisheryIds]() {return fisheryIds; });
	}

	void Ship::SetHeading(double heading) const
	{
		game::Ship::SetHeading(shipId, heading);
	}

	void Ship::SetSpeed(double speed) const
	{
		game::Ship::SetSpeed(shipId, speed);
	}
	
	ship::Fouling Ship::GetFouling(const game::Side& side) const
	{
		return ship::Fouling(shipId, side);
	}

	static std::optional<std::string> TryGetName(int shipId)
	{
		return game::Ship::GetName(shipId);
	}

	std::string Ship::GetName() const
	{
		return TryGetName(shipId).value();
	}

	void Ship::SetName(const std::string& name) const
	{
		game::Ship::SetName(shipId, name);
	}
}