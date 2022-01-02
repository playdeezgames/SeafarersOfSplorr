#pragma once
#include "Game.Session.World.Calendar.Month.h"
#include <list>
namespace game::session::world::calendar
{
	struct Months
	{
		constexpr explicit Months(int worldId) : worldId(worldId) {}
		constexpr Month GetMonth(int ordinal) const { return Month(worldId, ordinal); }
		std::list<Month> GetAll() const;
	private:
		int worldId;
	};
}