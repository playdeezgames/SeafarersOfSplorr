#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.Avatar.h>
#include "Game.Avatar.AtSea.h"
#include "Game.Avatar.Items.h"
#include "Game.Avatar.Ship.h"
#include "Game.Avatar.State.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Ships.h"
#include "Game.World.h"
namespace game::avatar::AtSea
{
	void Reset(const game::Difficulty&)
	{
		auto worldSize = game::World::GetSize();
		data::game::Avatar::AvatarData data =
		{
			{worldSize.GetX() / 2.0, worldSize.GetY() / 2.0},
			common::Data::ModuloDouble(common::RNG::FromRange(0.0, common::Heading::DEGREES), common::Heading::DEGREES).value(),
			1.0
		};
		data::game::Avatar::Write(data);
		data::game::Avatar::SetState((int)game::avatar::State::AT_SEA);
	}

	double GetHeading()
	{
		return data::game::Avatar::Read().value().heading;
	}

	void SetHeading(double heading)
	{
		auto data = data::game::Avatar::Read().value();
		data.heading = common::Data::ModuloDouble(heading, common::Heading::DEGREES).value();
		data::game::Avatar::Write(data);
	}

	const double SPEED_MINIMUM = 0.0;
	const double SPEED_MAXIMUM = 1.0;

	common::XY<double> GetLocation()
	{
		return data::game::Avatar::Read().value().location;
	}

	double GetSpeed()
	{
		return data::game::Avatar::Read().value().speed;
	}

	void SetSpeed(double speed)
	{
		auto data = data::game::Avatar::Read().value();
		data.speed = common::Data::ClampDouble(speed, SPEED_MINIMUM, SPEED_MAXIMUM);
		data::game::Avatar::Write(data);
	}

	static void ApplyHunger()
	{
		const double SATIETY_DELTA = -1.0;
		const double HEALTH_DELTA = -1.0;
		if (!game::avatar::Statistics::IsStarving())
		{
			game::avatar::Statistics::ChangeSatiety(SATIETY_DELTA);
		}
		else
		{
			game::avatar::Statistics::ChangeHealth(HEALTH_DELTA);
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

	static void ApplyTurnEffects()
	{
		game::avatar::Statistics::SpendTurn();
		ApplyHunger();
		ApplyEating();
		ApplyWindChange();
	}

	static common::XY<double> ClampAvatarLocation(const common::XY<double>& candidate, MoveResult& result)
	{
		auto nextLocation = candidate;
		auto worldSize = game::World::GetSize();
		if (nextLocation.GetX() < 0.0)
		{
			result = MoveResult::CLAMPED;
			nextLocation = { 0, nextLocation.GetY() };
		}
		if (nextLocation.GetX() > worldSize.GetX())
		{
			result = MoveResult::CLAMPED;
			nextLocation = { worldSize.GetX(), nextLocation.GetY() };
		}
		if (nextLocation.GetY() < 0.0)
		{
			result = MoveResult::CLAMPED;
			nextLocation = { nextLocation.GetX(), 0.0 };
		}
		if (nextLocation.GetY() > worldSize.GetY())
		{
			result = MoveResult::CLAMPED;
			nextLocation = { nextLocation.GetX(), worldSize.GetY() };
		}
		return nextLocation;
	}

	MoveResult Move()
	{
		MoveResult result = MoveResult::MOVED;
		auto avatar = data::game::Avatar::Read().value();

		auto effectiveSpeed = avatar.speed;
		auto relativeHeading = common::Heading::Difference(game::World::GetWindHeading(), avatar.heading);
		double multiplier = 1.0 - std::abs(relativeHeading / common::Heading::DEGREES);

		auto shipDescriptor = game::Ships::Read(game::avatar::Ship::Read());
		common::XY<double> delta =
			common::Heading::DegreesToXY(avatar.heading) *
			effectiveSpeed * multiplier *
			shipDescriptor.properties.find(game::ship::Property::SPEED_FACTOR)->second;

		avatar.location = ClampAvatarLocation(avatar.location + delta, result);

		ApplyTurnEffects();
		data::game::Avatar::Write(avatar);
		return result;
	}
}