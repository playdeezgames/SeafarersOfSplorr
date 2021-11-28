#pragma once
#include "Game.Quest.h"
#include <optional>
namespace game::character
{
	enum class AcceptQuestResult
	{
		ACCEPTED_QUEST,
		NO_QUEST_TO_ACCEPT,
		ALREADY_HAS_QUEST
	};
	struct Quest
	{
		static std::optional<game::Quest> Read(int);
		static AcceptQuestResult Accept(int,int);
		static bool Complete(int,int);
		static bool Abandon(int);
	};
}