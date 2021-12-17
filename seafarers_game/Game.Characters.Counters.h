#pragma once
#include "Game.Characters.Counter.h"
#include <optional>
#include <string>
namespace game::characters
{
	struct Counters
	{
		static void Write(int, const Counter&, int);
		static std::optional<int> Read(int, const Counter&);
	};
}
