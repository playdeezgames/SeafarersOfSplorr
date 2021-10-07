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
	void AtSea::Reset(const game::Difficulty&)
	{
		auto worldSize = game::World::GetSize();
		data::game::Avatar data =
		{
			0,
			"nada"//TODO: generate a name?
		};
		data::game::Avatar::Write(data);
		auto shipType = game::ShipTypes::GenerateForAvatar();
		int shipId = game::Ship::Add(shipType, game::ShipNames::Generate(), { worldSize.GetX() / 2.0, worldSize.GetY() / 2.0 }, common::Data::ModuloDouble(common::RNG::FromRange(0.0, common::Heading::DEGREES), common::Heading::DEGREES).value(), 1.0);
		data::game::avatar::Ship::Write(data::game::Player::GetAvatarId(), shipId);
		data::game::Avatar::WriteState((int)game::avatar::State::AT_SEA);
	}


	static void ApplyHunger()
	{
		double delta = -1.0;
		if (Plights::Has(Plight::DOUBLE_HUNGER) && !Plights::Has(Plight::HUNGER_IMMUNITY))
		{
			delta *= 2.0;
		}
		if (Plights::Has(Plight::HUNGER_IMMUNITY) && !Plights::Has(Plight::DOUBLE_HUNGER))
		{
			delta = 0.0;
		}
		if (!game::avatar::Statistics::IsStarving())
		{
			game::avatar::Statistics::ChangeSatiety(delta);
		}
		else
		{
			game::avatar::Statistics::ChangeHealth(delta);
		}
	}

	static void ApplyEating()
	{
		const double EAT_BENEFIT = 10.0;
		if (game::avatar::Statistics::NeedToEat(EAT_BENEFIT))
		{
			auto rations = game::avatar::Items::Read(game::Item::RATIONS);
			if (rations > 0)
			{
				game::avatar::Statistics::Eat(EAT_BENEFIT);
				game::avatar::Items::Remove(game::Item::RATIONS, 1);
			}
		}
	}

	static void ApplyWindChange()
	{
		const double NORMAL_WIND_CHANGE = 5.0;
		game::World::SetWindHeading(game::World::GetWindHeading()+common::RNG::FromRange(-NORMAL_WIND_CHANGE, NORMAL_WIND_CHANGE));
	}

	static void ApplyTurn()
	{
		if (Plights::Has(Plight::DOUBLE_AGING) && !Plights::Has(Plight::AGING_IMMUNITY))
		{
			game::avatar::Statistics::SpendTurn();
			game::avatar::Statistics::SpendTurn();
			return;
		}
		if (Plights::Has(Plight::AGING_IMMUNITY) && !Plights::Has(Plight::DOUBLE_AGING))
		{
			//do nothing!
			return;
		}
		game::avatar::Statistics::SpendTurn();
	}

	void AtSea::ApplyTurnEffects()
	{
		ApplyTurn();
		ApplyHunger();
		ApplyEating();
		ApplyWindChange();
	}

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