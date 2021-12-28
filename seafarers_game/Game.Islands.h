#pragma once
#include "Game.Difficulty.h"
#include "Game.Island.h"
#include <list>
#include <optional>
#include <string>
namespace game
{
	struct Islands
	{
		static std::optional<Island> Read(int, int);
		static const std::string UNKNOWN;
	};
}
