#pragma once
#include "Game.Characters.Counter.h"
#include <string>
namespace game::characters
{
	struct Counters
	{
		static const std::string& GetName(const Counter&);
	};
}
