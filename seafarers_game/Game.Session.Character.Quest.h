#pragma once
#include "Game.Session.Island.h"
#include <optional>
#include <string>
namespace game::session::character
{
	struct Quest
	{
		Quest(int);
		Island GetDestinationIsland() const;
		std::string GetCompletionMessage() const;
		void Abandon() const;
	private:
		int characterId;
	};
}
