#pragma once
#include "Game.Avatar.State.h"
#include <string>
namespace game::avatar::Docked
{
	struct DockedStateTransition
	{
		std::string logColor;
		std::string logText;
		State dockedState;
	};
}

