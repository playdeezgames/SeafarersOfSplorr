#pragma once
#include "Game.Character.State.h"
#include "Game.Difficulty.h"
#include "Game.Item.h"
#include <optional>
#include <string>
namespace game
{
	struct Character
	{
		static void ApplyTurnEffects();
		static void Reset(const Difficulty&);
		static std::optional<std::string> GetName(int);
	};
}