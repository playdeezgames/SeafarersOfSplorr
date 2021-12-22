#pragma once
#include <string>
namespace game::session::world::calendar
{
	struct Month
	{
		constexpr Month(int worldId, int ordinal) : worldId(worldId), ordinal(ordinal) {}
		std::string GetName() const;
		int GetDays() const;
	private:
		int worldId;
		int ordinal;
	};
}
