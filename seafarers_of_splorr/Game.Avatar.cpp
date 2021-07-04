#include "Game.Avatar.h"
#include "Data.Game.Avatar.h"
#include "Common.Utility.h"
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

	const double HEADING_MAXIMUM = common::Utility::DEGREES;

	void SetHeading(double heading)
	{
		auto data = data::game::Avatar::Read().value();
		data.heading = common::Utility::ModuloDouble(heading, HEADING_MAXIMUM);
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
		data.speed = common::Utility::ClampDouble(speed, SPEED_MINIMUM, SPEED_MAXIMUM);
		data::game::Avatar::Write(data);
	}

	void Reset(const game::Difficulty&)
	{
		data::game::Avatar::AvatarData data =
			{
				{0.0, 0.0},
				0.0,
				1.0
			};
		data::game::Avatar::Write(data);
	}
}