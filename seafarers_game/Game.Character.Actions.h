#pragma once
#include "Game.Character.Action.h"
#include "Game.Character.State.h"
#include <optional>
namespace game::character
{
	struct Actions
	{
		static void DoAction(int,const Action&);
	};
}