#pragma once
namespace game::session::world
{
	constexpr double VIEW_DISTANCE = 10.0;
	constexpr double DOCK_DISTANCE = 1.0;
	struct Distances
	{
		constexpr explicit Distances(int worldId) : worldId(worldId) {}
		constexpr double GetView() const { return VIEW_DISTANCE; }
		constexpr double GetDock() const { return DOCK_DISTANCE; }
		double GetIslandSpacing() const;
		void Reset() const {}
	private:
		int worldId;
	};
}
