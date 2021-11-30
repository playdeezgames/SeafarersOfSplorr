#pragma once
#include "Game.Ship.DockResult.h"
#include "Game.Difficulty.h"
#include <optional>
namespace game::ship
{
	struct Docked
	{
		static std::optional<DockResult> Dock(int);
		static std::optional<int> GetIsland(int);
	};
}
