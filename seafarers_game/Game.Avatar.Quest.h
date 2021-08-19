#pragma once
#include "Game.Quest.h"
#include <optional>
namespace game::avatar::Quest
{
	enum class AcceptQuestResult
	{
		ACCEPTED_QUEST,
		NO_QUEST_TO_ACCEPT,
		ALREADY_HAS_QUEST
	};
	std::optional<game::Quest::QuestModel> Read();
	AcceptQuestResult AcceptQuest(const common::XY<double>&);
	bool CompleteQuest(const common::XY<double>&);
	bool AbandonQuest();
}