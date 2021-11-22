#pragma once
#include "Game.Character.State.h"
#include "Game.Difficulty.h"
#include "Game.Item.h"
#include <optional>
#include <string>
namespace game
{
	struct Avatar
	{
		static void ApplyTurnEffects();
		static void Reset(const Difficulty&, int);
		static std::optional<std::string> GetName(int);
	};
}
