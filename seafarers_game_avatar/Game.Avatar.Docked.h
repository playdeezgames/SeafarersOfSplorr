#pragma once
#include <Common.XY.h>
#include "Game.Character.Action.h"
#include "Game.Avatar.DockResult.h"
#include "Game.Character.State.h"
#include "Game.Difficulty.h"
#include <optional>
namespace game::avatar//20211017
{
	struct Docked
	{
		static void Reset(const game::Difficulty&);
		static std::optional<DockResult> Dock();
		static std::optional<common::XY<double>> ReadLocation();
	};

}
