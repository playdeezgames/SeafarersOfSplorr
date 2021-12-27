#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Characters.h"
#include "Game.Session.Demigods.h"
#include "Game.Session.Fisheries.h"
#include "Game.Session.Items.h"
#include "Game.Session.Player.h"
#include "Game.Session.Saves.h"
#include "Game.Session.Ships.h"
#include "Game.Session.World.h"
#include <optional>
namespace game
{
	constexpr int WORLD_ID = 1;
	struct Session
	{
		constexpr Session() {}

		constexpr session::Characters GetCharacters() const { return session::Characters(); }
		constexpr session::Items GetItems() const { return session::Items(); }
		constexpr session::Player GetPlayer() const { return session::Player(); }
		constexpr session::Saves GetSaves() const { return session::Saves(); }
		constexpr session::Ships GetShips() const { return session::Ships(); }
		constexpr session::World GetWorld() const { return session::World(WORLD_ID); }

		session::Fisheries GetFisheries() const;//to world

		void ApplyTurnEffects() const;
		void Reset() const;
		void Populate(const Difficulty&) const;
	};
}
