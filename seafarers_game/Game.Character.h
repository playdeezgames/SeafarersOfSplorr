#pragma once
#include "Game.Difficulty.h"
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
