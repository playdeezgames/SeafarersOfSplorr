#pragma once
namespace game::session::world
{
	struct Calendar
	{
		constexpr Calendar(int worldId) : worldId(worldId) {}
		int GetYearLength() const;
	private:
		int worldId;
	};
}
