#pragma once
namespace game::session::world
{
	struct Distances
	{
		Distances() = delete;
		static constexpr double GetView() { return 10.0; }
		static constexpr double GetDock() { return 1.0; }
		static double GetIslandSpacing();
	};
}
