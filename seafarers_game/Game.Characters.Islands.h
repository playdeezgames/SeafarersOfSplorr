#pragma once
#include "Game.Characters.State.h"
#include <list>
#include <optional>
namespace game::characters
{
	struct Islands
	{
		static std::optional<int> Read(int);
		static void Write(int, int);
		static void Clear(int);
		static std::list<int> All(int, const game::characters::State&);
	};
}
