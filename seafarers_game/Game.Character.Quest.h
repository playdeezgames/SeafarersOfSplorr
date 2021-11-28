#pragma once
#include "Game.Character.AcceptQuestResult.h"
#include "Game.Quest.h"
#include <optional>
namespace game::character
{
	struct Quest
	{
		static std::optional<game::Quest> Read(int);
		static AcceptQuestResult Accept(int,int);
		static bool Complete(int,int);
		static bool Abandon(int);
	};
}