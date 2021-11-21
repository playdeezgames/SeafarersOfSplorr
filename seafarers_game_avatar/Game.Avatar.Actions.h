#pragma once
#include "Game.Avatar.Action.h"
#include "Game.Avatar.State.h"
namespace game::avatar
{
	struct Actions
	{
		static std::optional<avatar::State> GetState(int);
		static bool DoAction(int,const avatar::Action&);
	};
}