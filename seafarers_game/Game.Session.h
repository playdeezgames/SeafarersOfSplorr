#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Player.h"
#include <optional>
namespace game
{
	struct Session
	{
		constexpr Session() {}

		constexpr session::Player GetPlayer() const { return session::Player(); }

		void ApplyTurnEffects() const;
		void Reset() const;
		void Populate(const Difficulty& difficulty) const;
	};
}
