#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Characters.h"
#include "Game.Session.Demigods.h"
#include "Game.Session.Islands.h"
#include "Game.Session.Items.h"
#include "Game.Session.ItemTypes.h"
#include "Game.Session.Ships.h"
#include "Game.Session.World.h"
#include "Game.Session.Fisheries.h"
#include "Game.Session.Saves.h"
#include <optional>
namespace game
{
	struct Session
	{
		session::Characters GetCharacters() const;
		session::Character GetPlayerCharacter() const;
		session::Ships GetShips() const;
		session::World GetWorld() const;
		session::Islands GetIslands() const;
		session::Items GetItems() const;
		session::ItemTypes GetItemTypes() const;
		session::Demigods GetDemigods() const;
		session::Fisheries GetFisheries() const;
		session::Saves GetSaves() const;
		void ApplyTurnEffects() const;
		void Reset(const Difficulty&) const;
	};
}
