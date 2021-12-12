#pragma once
#include "Game.Session.Island.h"
#include <optional>
namespace game::session::character
{
	struct Quest
	{
		Quest(int);
		std::optional<Island> GetDestinationIsland() const;
	private:
		int characterId;
	};
}
