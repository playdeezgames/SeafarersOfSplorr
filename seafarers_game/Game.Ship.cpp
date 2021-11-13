#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.Utility.h>
#include <Common.Utility.List.h>
#include <Common.Utility.Optional.h>
#include <Data.Game.Avatar.Ship.h>
#include <Data.Game.Ship.h>
#include <Data.Game.Ship.Statistic.h>
#include "Game.h"
#include <Game.Avatar.Ship.h>
#include "Game.Ship.h"
#include "Game.Avatar.ShipStatistics.h"
#include "Game.ShipTypes.h"
#include "Game.World.h"
namespace game//20211017
{
	std::optional<ShipType> Ship::GetShipType(int shipId)
	{
		return 
			common::utility::Optional::Map<data::game::Ship, ShipType>(
				data::game::Ship::Read(shipId), 
				[](const data::game::Ship& ship) {return (ShipType)ship.shipType; });
	}

	static void AddShipStatistics(int shipId, ShipType shipType)
	{
		common::utility::List::Iterate<ShipStatistic>(
			ShipTypes::GetStatistics(shipType),
			[shipId, shipType](const ShipStatistic& statistic) 
			{
				data::game::ship::Statistic::Write(shipId,
					(int)statistic,
					{
						ShipTypes::GetMinimumStatistic(shipType, statistic),
						ShipTypes::GetMaximumStatistic(shipType, statistic),
						ShipTypes::GetInitialStatistic(shipType, statistic)
					});
			});
	}

	int Ship::Add(const Ship& ship)
	{
		int shipId = data::game::Ship::NextId();
		data::game::Ship::Write({
			shipId,
			(int)ship.shipType,
			ship.name,
			ship.location,
			ship.heading,
			ship.speed
			});
		AddShipStatistics(shipId, ship.shipType);
		return shipId;
	}

	static data::game::Ship GetAvatarShip()//TODO: making a lot of assumptions about optionals!
	{
		return data::game::Ship::Read(
			game::avatar::Ship::Read().value().shipId).value();
	}

	std::optional<std::string> Ship::GetName(int shipId)
	{
		auto ship = data::game::Ship::Read(shipId);
		if (ship)
		{
			return ship.value().name;
		}
		return std::nullopt;
	}

	std::optional<double> Ship::GetHeading(int shipId)
	{
		auto ship = data::game::Ship::Read(shipId);
		if (ship)
		{
			return ship.value().heading;
		}
		return std::nullopt;
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

	static Ship::MoveResult ClampAvatarLocation(common::XY<double>& candidate)
	{
		return 
			(game::World::ClampLocation(candidate)) ?
			(Ship::MoveResult::CLAMPED) : 
			(Ship::MoveResult::MOVED);
	}

	static void HandleFouling(double speed)
	{
		avatar::ShipStatistics::IncreaseFouling(speed);
	}

	static double GetEffectiveSpeed(int shipId, double heading, double speed)
	{
		auto fouling = avatar::ShipStatistics::GetFouling();
		auto effectiveSpeed = speed * (1.0 - fouling);

		effectiveSpeed *= World::GetWindSpeedMultiplier(heading);

		auto shipType = game::Ship::GetShipType(shipId).value();
		effectiveSpeed *= game::ShipTypes::GetSpeedFactor(shipType);

		return effectiveSpeed;
	}

	Ship::MoveResult Ship::Move()
	{
		auto ship = GetAvatarShip();

		ship.location += 
			common::Heading::DegreesToXY(ship.heading) *
			GetEffectiveSpeed(ship.shipId, ship.heading, ship.speed);

		MoveResult result = ClampAvatarLocation(ship.location);

		HandleFouling(ship.speed);

		data::game::Ship::Write(ship);

		game::ApplyTurnEffects();
		return result;
	}
}