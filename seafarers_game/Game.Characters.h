#pragma once
#include "Game.Characters.State.h"
#include "Game.Characters.Action.h"
#include "Game.Difficulty.h"
#include <optional>
#include <string>
namespace game
{
	struct Characters
	{
		static void ApplyTurnEffects();
		static void Reset(const Difficulty&);
		static std::optional<std::string> GetName(int);
		static std::optional<characters::State> GetState(int);
		static void DoAction(int, const characters::Action&);
		static int Create(const game::characters::State&);
	private:
		static void SetState(int, const characters::State&);
	};
}
