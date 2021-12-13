#pragma once
#include "Game.Characters.AcceptQuestResult.h"
#include "Game.Quest.h"
#include <optional>
namespace game::characters
{
	struct Quests
	{
		static std::optional<game::Quest> Read(int);
		static AcceptQuestResult Accept(int,int);
		static bool Complete(int,int);
		static bool Abandon(int);
	};
}