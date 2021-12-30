#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.Utility.List.h>
#include <Common.Utility.Optional.h>
#include <Data.Game.Ship.h>
#include "Game.Session.h"
#include "Game.Ship.h"
namespace game
{
	using ShipData = data::game::Ship;

	const double SPEED_MINIMUM = 0.0;
	const double SPEED_MAXIMUM = 1.0;

	static double GetEffectiveSpeed(int shipId, double heading, double speed)
	{
		auto fouling = 0.0;
		auto effectiveSpeed = speed * (1.0 - fouling);

		effectiveSpeed *= game::Session().GetWorld().GetWind().GetMultiplier(heading);

		effectiveSpeed *= game::Session().GetShips().GetShip(shipId).GetShipType().GetSpeedFactor();

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

			//TODO: Handle Fouling

			ShipData::SetLocation(shipId, location.value());
		}
	}
}