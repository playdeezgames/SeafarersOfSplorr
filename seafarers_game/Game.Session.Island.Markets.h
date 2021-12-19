#pragma once
namespace game::session::island
{
	struct Markets
	{
		constexpr Markets(int islandId) : islandId(islandId) {}
	private:
		int islandId;
	};
}
