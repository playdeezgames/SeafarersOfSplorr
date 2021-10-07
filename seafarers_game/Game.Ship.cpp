#include <Common.Data.h>
#include <Common.Heading.h>
#include <Data.Game.Ship.h>
#include <Data.Game.Ship.Statistic.h>
#include <Game.Avatar.Ship.h>
#include "Game.Ship.h"
#include "Game.ShipTypes.h"
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
				(int)statistic.first, 
				{
					statistic.second.minimum,
					statistic.second.maximum,
					statistic.second.initial
				});
		}
		return shipId;
	}

	std::string Ship::GetName()
	{
		auto shipId = game::avatar::Ship::Read().value();
		auto ship = data::game::Ship::Read(shipId).value();
		return ship.name;
	}

	double Ship::GetHeading()
	{
		auto shipId = game::avatar::Ship::Read().value();
		auto ship = data::game::Ship::Read(shipId).value();
		return ship.heading;
	}

	void Ship::SetHeading(double heading)
	{
		auto shipId = game::avatar::Ship::Read().value();
		auto ship = data::game::Ship::Read(shipId).value();
		ship.heading = common::Data::ModuloDouble(heading, common::Heading::DEGREES).value();
		data::game::Ship::Write(ship);
	}

	const double SPEED_MINIMUM = 0.0;
	const double SPEED_MAXIMUM = 1.0;

	common::XY<double> Ship::GetLocation()
	{
		auto shipId = game::avatar::Ship::Read().value();
		auto ship = data::game::Ship::Read(shipId).value();
		return ship.location;
	}

	double Ship::GetSpeed()
	{
		auto shipId = game::avatar::Ship::Read().value();
		auto ship = data::game::Ship::Read(shipId).value();
		return ship.speed;
	}

	void Ship::SetSpeed(double speed)
	{
		auto shipId = game::avatar::Ship::Read().value();
		auto ship = data::game::Ship::Read(shipId).value();
		ship.speed = common::Data::ClampDouble(speed, SPEED_MINIMUM, SPEED_MAXIMUM);
		data::game::Ship::Write(ship);
	}

}