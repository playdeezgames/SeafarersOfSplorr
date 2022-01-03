#pragma once
namespace game::session::world
{
	struct Distances
	{
		constexpr explicit Distances(int worldId) : worldId(worldId) {}
		constexpr double GetView() const { return 10.0; }
		constexpr double GetDock() const { return 1.0; }
		double GetIslandSpacing() const;
		void Reset() const {}
	private:
		int worldId;
	};
}
