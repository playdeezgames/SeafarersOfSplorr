#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.Utility.h>
#include <Common.Utility.List.h>
#include <Common.Utility.Optional.h>
#include <Data.Game.Character.Ship.h>
#include <Data.Game.Ship.h>
#include <Data.Game.Ship.Statistic.h>
#include "Game.h"
#include "Game.Character.Ship.h"
#include "Game.Ship.h"
#include "Game.Character.ShipStatistics.h"
#include "Game.ShipTypes.h"
#include "Game.World.h"
namespace game
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
		int shipId = data::game::Ship::Write({
			0,
			(int)ship.shipType,
			ship.name,
			ship.location,
			ship.heading,
			ship.speed
			});
		AddShipStatistics(shipId, ship.shipType);
		return shipId;
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

	void Ship::SetName(int shipId, const std::string& name)
	{
		auto ship = data::game::Ship::Read(shipId);
		if (ship)
		{
			ship.value().name = name;
			data::game::Ship::Write(ship.value());
		}
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

	void Ship::SetHeading(int shipId, double heading)
	{
		auto ship = data::game::Ship::Read(shipId);
		if (ship)
		{
			ship.value().heading = common::Data::ModuloDouble(heading, common::Heading::DEGREES).value();
			data::game::Ship::Write(ship.value());
		}
	}

	const double SPEED_MINIMUM = 0.0;
	const double SPEED_MAXIMUM = 1.0;

	std::optional<common::XY<double>> Ship::GetLocation(int shipId)
	{
		auto ship = data::game::Ship::Read(shipId);
		if (ship)
		{
			return ship.value().location;
		}
		return std::nullopt;
	}

	std::optional<double> Ship::GetSpeed(int shipId)
	{
		auto ship = data::game::Ship::Read(shipId);
		if (ship)
		{
			return ship.value().speed;
		}
		return std::nullopt;
	}

	void Ship::SetSpeed(int shipId, double speed)
	{
		auto ship = data::game::Ship::Read(shipId);
		if (ship)
		{
			ship.value().speed = common::Data::ClampDouble(speed, SPEED_MINIMUM, SPEED_MAXIMUM);
			data::game::Ship::Write(ship.value());
		}
	}

	static void HandleFouling(double speed)
	{
		character::ShipStatistics::IncreaseFouling(speed);
	}

	static double GetEffectiveSpeed(int shipId, double heading, double speed)
	{
		auto fouling = character::ShipStatistics::GetFouling();
		auto effectiveSpeed = speed * (1.0 - fouling);

		effectiveSpeed *= World::GetWindSpeedMultiplier(heading);

		auto shipType = game::Ship::GetShipType(shipId).value();
		effectiveSpeed *= game::ShipTypes::GetSpeedFactor(shipType);

		return effectiveSpeed;
	}

	void Ship::ApplyTurnEffects(int shipId)
	{
		auto ship = data::game::Ship::Read(shipId);
		if (ship)
		{
			ship.value().location += 
				common::Heading::DegreesToXY(ship.value().heading) *
				GetEffectiveSpeed(ship.value().shipId, ship.value().heading, ship.value().speed);

			game::World::ClampLocation(ship.value().location);

			HandleFouling(ship.value().speed);

			data::game::Ship::Write(ship.value());
		}
	}
}