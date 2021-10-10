#include <Common.Data.h>
#include <Common.Heading.h>
#include <Data.Game.Avatar.Ship.h>
#include <Data.Game.Ship.h>
#include <Data.Game.Ship.Statistic.h>
#include "Game.h"
#include <Game.Avatar.Ship.h>
#include "Game.Ship.h"
#include "Game.Avatar.ShipStatistics.h"
#include "Game.ShipTypes.h"
#include "Game.World.h"
namespace game
{
	std::optional<ShipType> Ship::GetShipType(int shipId)
	{
		auto ship = data::game::Ship::Read(shipId);
		if (ship)
		{
			return (ShipType)ship.value().shipType;
		}
		return std::nullopt;
	}

	int Ship::Add(const ShipType& shipType, const std::string& name, const common::XY<double> location, double heading, double speed)
	{
		int shipId = data::game::Ship::NextId();
		data::game::Ship::Write({
			shipId,
			(int)shipType,
			name,
			location,
			heading,
			speed
			});
		auto statistics = game::ShipTypes::GetStatistics(shipType);
		for (auto statistic : statistics)
		{
			data::game::ship::Statistic::Write(shipId, 
				(int)statistic, 
				{
					game::ShipTypes::GetMinimumStatistic(shipType, statistic),
					game::ShipTypes::GetMaximumStatistic(shipType, statistic),
					game::ShipTypes::GetInitialStatistic(shipType, statistic)
				});
		}
		return shipId;
	}

	static data::game::Ship GetAvatarShip()
	{
		auto shipId = game::avatar::Ship::Read().value().shipId;
		return data::game::Ship::Read(shipId).value();
	}

	std::string Ship::GetName()
	{
		return GetAvatarShip().name;
	}

	double Ship::GetHeading()
	{
		return GetAvatarShip().heading;
	}

	void Ship::SetHeading(double heading)
	{
		auto ship = GetAvatarShip();
		ship.heading = common::Data::ModuloDouble(heading, common::Heading::DEGREES).value();
		data::game::Ship::Write(ship);
	}

	const double SPEED_MINIMUM = 0.0;
	const double SPEED_MAXIMUM = 1.0;

	common::XY<double> Ship::GetLocation()
	{
		return GetAvatarShip().location;
	}

	double Ship::GetSpeed()
	{
		return GetAvatarShip().speed;
	}

	void Ship::SetSpeed(double speed)
	{
		auto ship = GetAvatarShip();
		ship.speed = common::Data::ClampDouble(speed, SPEED_MINIMUM, SPEED_MAXIMUM);
		data::game::Ship::Write(ship);
	}
	static common::XY<double> ClampAvatarLocation(const common::XY<double>& candidate, Ship::MoveResult& result)
	{
		auto nextLocation = candidate;
		auto worldSize = game::World::GetSize();
		if (nextLocation.GetX() < 0.0)
		{
			result = Ship::MoveResult::CLAMPED;
			nextLocation = { 0, nextLocation.GetY() };
		}
		if (nextLocation.GetX() > worldSize.GetX())
		{
			result = Ship::MoveResult::CLAMPED;
			nextLocation = { worldSize.GetX(), nextLocation.GetY() };
		}
		if (nextLocation.GetY() < 0.0)
		{
			result = Ship::MoveResult::CLAMPED;
			nextLocation = { nextLocation.GetX(), 0.0 };
		}
		if (nextLocation.GetY() > worldSize.GetY())
		{
			result = Ship::MoveResult::CLAMPED;
			nextLocation = { nextLocation.GetX(), worldSize.GetY() };
		}
		return nextLocation;
	}

	Ship::MoveResult Ship::Move()
	{
		MoveResult result = MoveResult::MOVED;

		auto fouling = avatar::ShipStatistics::GetFouling();
		auto ship = GetAvatarShip();
		avatar::ShipStatistics::IncreaseFouling(ship.speed);
		auto effectiveSpeed = ship.speed * (1.0 - fouling);

		double multiplier = World::GetWindSpeedMultiplier(ship.heading);

		auto shipType = game::Ship::GetShipType(ship.shipId).value();
		auto speedFactor = game::ShipTypes::GetSpeedFactor(shipType);

		common::XY<double> delta =
			common::Heading::DegreesToXY(ship.heading) *
			effectiveSpeed * multiplier *
			speedFactor;

		ship.location = ClampAvatarLocation(ship.location + delta, result);

		game::ApplyTurnEffects();
		data::game::Ship::Write(ship);
		return result;
	}
}