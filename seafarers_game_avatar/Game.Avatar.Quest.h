#pragma once
#include "Game.Quest.h"
#include <optional>
namespace game::avatar
{
	enum class AcceptQuestResult
	{
		ACCEPTED_QUEST,
		NO_QUEST_TO_ACCEPT,
		ALREADY_HAS_QUEST
	};
	struct Quest
	{
		static std::optional<game::Quest> Read();
		static AcceptQuestResult Accept(const common::XY<double>&);
		static bool Complete(const common::XY<double>&);
		static bool Abandon();
	};
}