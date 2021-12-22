#pragma once
namespace game::session::world
{
	struct Distances
	{
		constexpr Distances(int worldId) : worldId(worldId) {}
		double GetView() const;
		double GetDock() const;
		double GetIslandSpacing() const;
	private:
		int worldId;
	};
}
