#pragma once
#include "Game.Avatar.Action.h"
#include "Game.Avatar.State.h"
namespace game::avatar//20211017
{
	struct Actions
	{
		static std::optional<avatar::State> GetState();
		static bool DoAction(const avatar::Action&);
	};
}