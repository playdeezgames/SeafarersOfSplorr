#include "Game.Avatar.h"
#include "Data.Game.Avatar.h"
#include "Common.Utility.h"
#include "Application.UIState.h"
#include "Game.World.h"
#include "Common.RNG.h"
#include "Game.Heading.h"
#include "Common.Data.h"
#include "Game.Avatar.Statistics.h"
namespace game::Avatar
{
	common::XY<double> GetLocation()
	{
		return data::game::Avatar::Read().value().location;
	}

	double GetHeading()
	{
		return data::game::Avatar::Read().value().heading;
	}

	const double HEADING_MAXIMUM = game::Heading::DEGREES;

	void SetHeading(double heading)
	{
		auto data = data::game::Avatar::Read().value();
		data.heading = common::Data::ModuloDouble(heading, HEADING_MAXIMUM);
		data::game::Avatar::Write(data);
	}

	double GetSpeed()
	{
		return data::game::Avatar::Read().value().speed;
	}

	const double SPEED_MINIMUM = 0.0;
	const double SPEED_MAXIMUM = 1.0;

	void SetSpeed(double speed)
	{
		auto data = data::game::Avatar::Read().value();
		data.speed = common::Data::ClampDouble(speed, SPEED_MINIMUM, SPEED_MAXIMUM);
		data::game::Avatar::Write(data);
	}

	void Reset(const game::Difficulty&)
	{
		auto worldSize = game::World::GetSize();
		data::game::Avatar::AvatarData data =
			{
				{worldSize.GetX()/2.0, worldSize.GetY()/2.0},
				common::Data::ModuloDouble(common::RNG::FromRange(0.0, HEADING_MAXIMUM), HEADING_MAXIMUM),
				1.0
			};
		data::game::Avatar::Write(data);
	}

	static void ApplyTurnEffects()
	{
		const double TURN_DELTA = -1.0;
		const double HUNGER_DELTA = -1.0;
		const double HEALTH_DELTA = -1.0;
		game::avatar::Statistics::ChangeCurrent(game::avatar::Statistic::TURNS_REMAINING, TURN_DELTA);
		if (game::avatar::Statistics::GetCurrent(game::avatar::Statistic::HUNGER) > game::avatar::Statistics::GetMinimum(game::avatar::Statistic::HUNGER))
		{
			game::avatar::Statistics::ChangeCurrent(game::avatar::Statistic::HUNGER, HUNGER_DELTA);
		}
		else
		{
			game::avatar::Statistics::ChangeCurrent(game::avatar::Statistic::HEALTH, HEALTH_DELTA);
		}
	}

	void Move()
	{
		auto avatar = data::game::Avatar::Read().value();
		common::XY<double> delta = game::Heading::DegreesToXY(avatar.heading)  * avatar.speed;
		avatar.location = avatar.location + delta;
		ApplyTurnEffects();
		data::game::Avatar::Write(avatar);
	}
}