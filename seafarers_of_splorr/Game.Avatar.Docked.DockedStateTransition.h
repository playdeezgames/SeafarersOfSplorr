#pragma once
#include "Game.Avatar.DockedState.h"
#include <string>
namespace game::avatar::Docked
{
	struct DockedStateTransition
	{
		std::string logColor;
		std::string logText;
		DockedState dockedState;
	};
}

