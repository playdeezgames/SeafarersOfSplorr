#pragma once
#include "Game.Quest.h"
#include <optional>
namespace game::avatar::Quest
{
	std::optional<game::Quest::QuestModel> Read();
	bool AcceptQuest(const common::XY<double>&);
	bool CompleteQuest(const common::XY<double>&);
	bool AbandonQuest();
}