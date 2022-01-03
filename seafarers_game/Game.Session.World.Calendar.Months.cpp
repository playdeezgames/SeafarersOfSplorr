#include <algorithm>
#include <Data.Game.World.Month.h>
#include "Game.Session.World.Calendar.Months.h"
#include <iterator>
namespace game::session::world::calendar
{
	std::list<Month> Months::GetAll() const
	{
		std::list<Month> result;
		auto months = data::game::world::Month::AllOrdinals(worldId);
		std::transform(
			months.begin(),
			months.end(),
			std::back_inserter(result),
			[this](int ordinal) 
			{ 
				return GetMonth(ordinal); 
			});
		return result;
	}
}