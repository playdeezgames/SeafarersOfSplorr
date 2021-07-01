#pragma once
#include "Common.XY.h"
#include "Maze.hpp"
#include <functional>
#include "Maze.Door.h"
#include "Maze.Direction.h"
namespace game::world
{
	enum class Border
	{
		BREAKABLE_WALL,
		DOOR,
		LOCK,
		UNBREAKABLE_WALL,
		BROKEN_WALL
	};
}
namespace game::world::Borders
{
	enum class WallBumpResult
	{
		JUST_PAIN,
		WALL_FALLS
	};
	world::Border GetBorderAhead(const common::XY<size_t>&, const maze::Direction&);
	void SetBorderAhead(const common::XY<size_t>&, const maze::Direction&, const world::Border&);
	WallBumpResult BumpAhead(const common::XY<size_t>&, const maze::Direction&);
	world::Border GetBorderLeft(const common::XY<size_t>&, const maze::Direction&);
	world::Border GetBorderRight(const common::XY<size_t>&, const maze::Direction&);
	bool IsExitable(const common::XY<size_t>&);
	std::vector<common::XY<size_t>> GetDeadEnds(std::function<bool(const common::XY<size_t>&)>);
	bool CanSpawnAvatar(const common::XY<size_t>&);
	void UpdateBorders(const maze::Maze<maze::Direction, maze::Door>& maze);
}
