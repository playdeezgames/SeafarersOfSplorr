#pragma once
namespace game::session::world
{
	struct Calendar
	{
		constexpr Calendar(int worldId) : worldId(worldId) {}
	private:
		int worldId;
	};
}
