#include <Data.Game.World.Month.h>
#include "Game.Session.World.Calendar.Month.h"
namespace game::session::world::calendar
{
	std::string Month::GetName() const
	{
		return data::game::world::Month::ReadName(worldId, ordinal).value();
	}

	int Month::GetDays() const
	{
		return data::game::world::Month::ReadDays(worldId, ordinal).value();
	}
}