#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.Avatar.h>
#include <Data.Game.Avatar.Ship.h>
#include <Data.Game.Player.h>
#include <Data.Game.Ship.h>
#include "Game.h"
#include "Game.Avatar.AtSea.h"
#include "Game.Avatar.Items.h"
#include "Game.Avatar.Plights.h"
#include "Game.Avatar.Ship.h"
#include "Game.Avatar.ShipStatistics.h"
#include "Game.Avatar.State.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Islands.h"
#include <Game.Ship.h>
#include <Game.ShipNames.h>
#include "Game.ShipTypes.h"
#include "Game.World.h"
namespace game::avatar
{
	static common::XY<double> ClampAvatarLocation(const common::XY<double>& candidate, AtSea::MoveResult& result)
	{
		auto nextLocation = candidate;
		auto worldSize = game::World::GetSize();
		if (nextLocation.GetX() < 0.0)
		{
			result = AtSea::MoveResult::CLAMPED;
			nextLocation = { 0, nextLocation.GetY() };
		}
		if (nextLocation.GetX() > worldSize.GetX())
		{
			result = AtSea::MoveResult::CLAMPED;
			nextLocation = { worldSize.GetX(), nextLocation.GetY() };
		}
		if (nextLocation.GetY() < 0.0)
		{
			result = AtSea::MoveResult::CLAMPED;
			nextLocation = { nextLocation.GetX(), 0.0 };
		}
		if (nextLocation.GetY() > worldSize.GetY())
		{
			result = AtSea::MoveResult::CLAMPED;
			nextLocation = { nextLocation.GetX(), worldSize.GetY() };
		}
		return nextLocation;
	}

	AtSea::MoveResult AtSea::Move()
	{
		MoveResult result = MoveResult::MOVED;

		auto fouling = ShipStatistics::GetFouling();
		auto shipId = game::avatar::Ship::Read().value();
		auto ship = data::game::Ship::Read(shipId).value();
		ShipStatistics::IncreaseFouling(ship.speed);
		auto effectiveSpeed = ship.speed * (1.0 - fouling);

		double multiplier = World::GetWindSpeedMultiplier(ship.heading);

		auto shipType = game::Ship::GetShipType(shipId).value();
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