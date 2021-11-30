#pragma once
#include "Game.Character.DockResult.h"
#include "Game.Difficulty.h"
#include <optional>
namespace game::character
{
	struct Docked
	{
		static void Reset(const game::Difficulty&);
		static std::optional<DockResult> Dock(int);
		static std::optional<int> GetIsland(int);
	};
}
