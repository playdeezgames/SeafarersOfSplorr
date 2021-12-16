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
		static std::optional<std::string> GetName(int);
		static std::optional<characters::State> GetState(int);
		static void SetState(int, const characters::State&);
	};
}
