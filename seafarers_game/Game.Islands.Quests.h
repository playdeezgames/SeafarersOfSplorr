#pragma once
#include <optional>
#include "Game.Quest.h"
namespace game::islands::Quests
{
	void Update(const common::XY<double>&);
	std::optional<game::Quest::QuestModel> Read(const common::XY<double>&);
}
