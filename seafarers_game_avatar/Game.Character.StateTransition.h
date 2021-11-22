#pragma once
#include "Game.Character.State.h"
#include <string>
namespace game::character
{
	struct StateTransition
	{
		std::string color;
		std::string text;
		character::State state;
	};
}

