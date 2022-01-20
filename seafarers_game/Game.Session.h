#pragma once
#include "Game.Difficulty.h"
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

		constexpr session::Player GetPlayer() const { return session::Player(); }
		constexpr session::Saves GetSaves() const { return session::Saves(); }
		constexpr session::Ships GetShips() const { return session::Ships(); }
		constexpr session::World GetWorld() const { return session::World(WORLD_ID); }

		void ApplyTurnEffects() const;
		void Reset() const;
		void Populate(const Difficulty& difficulty) const;
	};
}
