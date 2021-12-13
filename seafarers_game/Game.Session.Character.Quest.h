#pragma once
#include "Game.Session.Island.h"
#include <optional>
#include <string>
namespace game::session::character
{
	struct Quest
	{
		Quest(int);
		std::optional<Island> GetDestinationIsland() const;
		std::optional<std::string> GetCompletionMessage() const;
		void Abandon() const;
	private:
		int characterId;
	};
}
