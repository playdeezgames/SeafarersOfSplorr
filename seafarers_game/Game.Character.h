#pragma once
#include "Game.Character.State.h"
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
		static std::optional<character::State> GetState(int);
		static void SetState(int, const character::State&);
	};
}
