#pragma once
#include "Game.Character.Action.h"
#include "Game.Character.State.h"
#include <optional>
namespace game::character
{
	struct Actions
	{
		static std::optional<State> GetState(int);
		static bool DoAction(int,const Action&);
	};
}