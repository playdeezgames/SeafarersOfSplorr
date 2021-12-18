#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.World.h>
#include "Game.Session.World.Wind.h"
namespace game::session::world
{
	double Wind::GetHeading() const
	{
		return data::game::World::Read().value().windHeading;
	}

	double Wind::GetMultiplier(double heading) const
	{
		auto relativeHeading = common::Heading::Difference(GetHeading(), heading);
		return 1.0 - std::abs(relativeHeading / common::Heading::DEGREES);
	}

	void Wind::SetHeading(double heading) const
	{
		auto data = data::game::World::Read();
		if (data)
		{
			data.value().windHeading = common::Data::ModuloDouble(heading, common::Heading::DEGREES).value();
			data::game::World::Write(data.value());
		}
	}

	void Wind::ApplyTurnEffects() const
	{
		const double NORMAL_WIND_CHANGE = 5.0;
		SetHeading(GetHeading() + common::RNG::FromRange(-NORMAL_WIND_CHANGE, NORMAL_WIND_CHANGE));
	}
}