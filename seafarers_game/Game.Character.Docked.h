#pragma once
#include <Common.XY.h>
#include "Game.Character.Action.h"
#include "Game.Character.DockResult.h"
#include "Game.Character.State.h"
#include "Game.Difficulty.h"
#include <optional>
namespace game::character
{
	struct Docked
	{
		static void Reset(const game::Difficulty&);
		static std::optional<DockResult> Dock(int);
		static std::optional<int> ReadLocation(int);
	};

}
