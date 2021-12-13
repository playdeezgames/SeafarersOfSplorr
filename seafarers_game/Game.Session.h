#pragma once
#include "Game.Session.Character.h"
#include "Game.Session.Ship.h"
#include "Game.Session.World.h"
#include <optional>
namespace game
{
	struct Session
	{
		session::Character GetCharacter(int) const;
		game::session::Character GetPlayerCharacter() const;
		game::session::Ship GetShip(int) const;
		session::World GetWorld() const;
		void ApplyTurnEffects() const;
	};
}
