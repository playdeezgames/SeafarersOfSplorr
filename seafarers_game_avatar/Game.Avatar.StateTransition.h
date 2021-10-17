#pragma once
#include "Game.Avatar.State.h"
#include <string>
namespace game::avatar//20211017
{
	struct StateTransition
	{
		std::string color;
		std::string text;
		State state;
	};
}

