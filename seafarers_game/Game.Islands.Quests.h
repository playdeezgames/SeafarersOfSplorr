#pragma once
#include "Game.Quest.h"
#include <optional>
namespace game::islands::Quests
{
	void Update(const common::XY<double>&);
	std::optional<game::Quest::QuestModel> Read(const common::XY<double>&);
}
