#pragma once
#include "Game.Quest.h"
#include <optional>
namespace game::islands//20211011
{
	struct Quests
	{
		static void Update(const common::XY<double>&);
		static std::optional<game::Quest> Read(const common::XY<double>&);
	};
}
