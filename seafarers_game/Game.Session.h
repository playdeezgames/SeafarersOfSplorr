#pragma once
#include "Game.Session.Characters.h"
#include "Game.Session.Items.h"
#include "Game.Session.Ship.h"
#include "Game.Session.World.h"
#include <optional>
namespace game
{
	struct Session
	{
		session::Characters GetCharacters() const;
		game::session::Character GetPlayerCharacter() const;
		game::session::Ship GetShip(int) const;//TODO: move to Ships subobject
		session::World GetWorld() const;
		session::Items GetItems() const;
		void ApplyTurnEffects() const;
	};
}
