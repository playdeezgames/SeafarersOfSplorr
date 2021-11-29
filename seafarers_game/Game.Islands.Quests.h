#pragma once
#include "Game.Quest.h"
#include <optional>
namespace game::islands
{
	struct Quests
	{
		static void Update(int, int);
		static std::optional<game::Quest> Read(int);
	};
}
