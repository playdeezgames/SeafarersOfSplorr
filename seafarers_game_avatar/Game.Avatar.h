#pragma once
#include "Game.Avatar.Action.h"
#include "Game.Avatar.State.h"
#include "Game.Difficulty.h"
#include <optional>
namespace game
{
	struct Avatar
	{
		static bool DoAction(const avatar::Action&);
		static std::optional<game::avatar::State> GetState();
		static void ApplyTurnEffects();
		static void Reset(const game::Difficulty&);
	};
}
