#pragma once
#include "Game.Characters.State.h"
#include <list>
#include <optional>
namespace game::characters
{
	struct Islands
	{
		static std::optional<int> Read(int);//gets the character's island association

		static std::list<int> All(int, const game::characters::State&);//all the characters on the island
	};
}
