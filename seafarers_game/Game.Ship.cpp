#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.Utility.List.h>
#include <Common.Utility.Optional.h>
#include <Data.Game.Ship.h>
#include <Data.Game.Ship.Statistic.h>
#include "Game.Session.h"
#include "Game.Ship.h"
#include "Game.Ship.Statistics.h"
#include "Game.ShipTypes.h"
namespace game
{
	using ShipData = data::game::Ship;

	std::optional<ShipType> Ship::GetShipType(int shipId)
	{
		return 
			common::utility::Optional::Map<int, ShipType>(
				ShipData::GetShipType(shipId),
				[](const int shipType) {return (ShipType)shipType; });
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
		int shipId = ShipData::Create(
			(int)ship.shipType,
			ship.name,
			ship.location,
			ship.heading,
			ship.speed);
		AddShipStatistics(shipId, ship.shipType);
		return shipId;
	}

	std::optional<std::string> Ship::GetName(int shipId)
	{
		return ShipData::GetName(shipId);
	}

	void Ship::SetName(int shipId, const std::string& name)
	{
		ShipData::SetName(shipId, name);
	}


	std::optional<double> Ship::GetHeading(int shipId)
	{
		return ShipData::GetHeading(shipId);
	}

	void Ship::SetHeading(int shipId, double heading)
	{
		ShipData::SetHeading(shipId, heading);
	}

	const double SPEED_MINIMUM = 0.0;
	const double SPEED_MAXIMUM = 1.0;

	std::optional<common::XY<double>> Ship::GetLocation(int shipId)
	{
		return ShipData::GetLocation(shipId);
	}

	std::optional<double> Ship::GetSpeed(int shipId)
	{
		return ShipData::GetSpeed(shipId);
	}

	void Ship::SetSpeed(int shipId, double speed)
	{
		ShipData::SetSpeed(shipId, speed);
	}

	static void HandleFouling(int shipId, double speed)
	{
		ship::Statistics::IncreaseFouling(shipId, speed);
	}

	static double GetEffectiveSpeed(int shipId, double heading, double speed)
	{
		auto fouling = ship::Statistics::GetFouling(shipId);
		auto effectiveSpeed = speed * (1.0 - fouling);

		effectiveSpeed *= game::Session().GetWorld().GetWind().GetMultiplier(heading);

		auto shipType = game::Ship::GetShipType(shipId).value();
		effectiveSpeed *= game::ShipTypes::GetSpeedFactor(shipType);

		return effectiveSpeed;
	}

	void Ship::ApplyTurnEffects(int shipId)
	{
		auto location = ShipData::GetLocation(shipId);
		if (location)
		{
			auto heading = ShipData::GetHeading(shipId).value();
			auto effectiveSpeed = GetEffectiveSpeed(shipId, heading, ShipData::GetSpeed(shipId).value());
			location.value() +=
				common::Heading::DegreesToXY(heading) *
				effectiveSpeed;

			game::Session().GetWorld().GetBounds().ClampLocation(location.value());

			HandleFouling(shipId, effectiveSpeed);

			ShipData::SetLocation(shipId, location.value());
		}
	}
}