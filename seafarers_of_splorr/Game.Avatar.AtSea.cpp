#include "Data.Game.Avatar.h"
#include "Common.Data.h"
#include "Game.Avatar.AtSea.h"
#include "Game.Heading.h"
namespace game::avatar::AtSea
{
	void Reset(const game::Difficulty&)
	{

	}

	double GetHeading()
	{
		return data::game::Avatar::Read().value().heading;
	}

	void SetHeading(double heading)
	{
		auto data = data::game::Avatar::Read().value();
		data.heading = common::Data::ModuloDouble(heading, game::Heading::DEGREES);
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
}