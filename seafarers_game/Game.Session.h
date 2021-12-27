#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Characters.h"
#include "Game.Session.Demigods.h"
#include "Game.Session.Fisheries.h"
#include "Game.Session.Items.h"
#include "Game.Session.Player.h"
#include "Game.Session.Saves.h"
#include "Game.Session.Ships.h"
#include "Game.Session.Tribes.h"
#include "Game.Session.World.h"
#include <optional>
namespace game
{
	constexpr int WORLD_ID = 1;
	struct Session
	{
		constexpr Session() {}

		session::Characters GetCharacters() const;//to world
		session::Fisheries GetFisheries() const;//to world
		session::Items GetItems() const;//to world
		session::Player GetPlayer() const;
		session::Saves GetSaves() const;
		session::Tribes GetTribes() const;//to world
		constexpr session::Ships GetShips() const { return session::Ships(); }//to world
		constexpr session::World GetWorld() const { return session::World(WORLD_ID); }

		void ApplyTurnEffects() const;
		void Reset() const;
		void Populate(const Difficulty&) const;
	};
}
