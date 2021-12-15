#pragma once
#include "Game.Session.Characters.h"
#include "Game.Session.Items.h"
#include "Game.Session.ItemTypes.h"
#include "Game.Session.Ships.h"
#include "Game.Session.World.h"
#include <optional>
namespace game
{
	struct Session
	{
		session::Characters GetCharacters() const;
		session::Character GetPlayerCharacter() const;
		session::Ships GetShips() const;
		session::World GetWorld() const;
		session::Items GetItems() const;
		session::ItemTypes GetItemTypes() const;
		void ApplyTurnEffects() const;
	};
}
