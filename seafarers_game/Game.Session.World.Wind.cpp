#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.World.h>
#include "Game.Session.World.Wind.h"
#include "Game.Session.World.h"
namespace game::session::world
{
	double Wind::GetHeading()
	{
		return data::game::World::ReadWindHeading(WORLD_ID).value();
	}

	double Wind::GetSpeedMultiplier(double heading)
	{
		auto relativeHeading = common::Heading::Difference(GetHeading(), heading);
		return 1.0 - std::abs(relativeHeading / common::Heading::DEGREES);
	}

	void Wind::SetHeading(double heading)
	{
		data::game::World::WriteWindHeading(WORLD_ID, common::Data::ModuloDouble(heading, common::Heading::DEGREES).value());
	}

	void Wind::ApplyTurnEffects()
	{
		const double NORMAL_WIND_CHANGE = 5.0;
		SetHeading(GetHeading() + common::RNG::FromRange(-NORMAL_WIND_CHANGE, NORMAL_WIND_CHANGE));
	}
}