#pragma once
#include "Game.Characters.State.h"
#include "Game.Characters.Action.h"
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
		static void DoAction(int, const character::Action&);
		static int Create(const game::character::State&);
	private:
		static void SetState(int, const character::State&);
	};
}
