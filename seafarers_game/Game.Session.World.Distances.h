#pragma once
namespace game::session::world
{
	struct Distances
	{
		constexpr Distances() {}
		double GetView() const;
		double GetDock() const;
		double GetIslandSpacing() const;
	};
}
