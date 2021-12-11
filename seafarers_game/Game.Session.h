#pragma once
#include "Game.Session.Character.h"
#include "Game.Session.Ship.h"
#include <optional>
namespace game
{
	struct Session
	{
		std::optional<game::session::Character> GetCharacter(int) const;
		std::optional<game::session::Character> GetPlayerCharacter() const;
	};
}
