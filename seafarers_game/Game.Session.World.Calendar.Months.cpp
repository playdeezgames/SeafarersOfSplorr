#include <Data.Game.World.Month.h>
#include "Game.Session.World.Calendar.Months.h"
namespace game::session::world::calendar
{
	std::list<Month> Months::GetAll() const
	{
		std::list<Month> result;
		auto months = data::game::world::Month::All(worldId);
		for (auto month : months)
		{
			result.push_back(GetMonth(month.ordinal));
		}
		return result;
	}
}