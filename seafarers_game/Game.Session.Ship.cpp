#include "Game.Ship.h"
#include "Game.Ship.Crew.h"
#include "Game.Session.Ship.h"
namespace game::session
{
	std::list<ship::Berth> Ship::GetBerths() const
	{
		std::list<ship::Berth> result;
		auto berths = game::ship::Crew::ReadForShip(shipId);
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

	static std::optional<common::XY<double>> TryGetLocation(int shipId)
	{
		return game::Ship::GetLocation(shipId);
	}

	common::XY<double> Ship::GetLocation() const
	{
		return TryGetLocation(shipId).value();
	}

	void Ship::SetHeading(double heading) const
	{
		game::Ship::SetHeading(shipId, heading);
	}

	void Ship::SetSpeed(double speed) const
	{
		game::Ship::SetSpeed(shipId, speed);
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