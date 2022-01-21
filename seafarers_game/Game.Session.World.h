#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Item.Type.h"
namespace game::session
{
	constexpr int WORLD_ID = 1;
	struct World
	{
		constexpr explicit World() : worldId(WORLD_ID) {}
		item::Type GetCurrencyItemSubtype() const;
		game::Difficulty GetDifficulty() const;
		void Reset() const;
		void Populate(const Difficulty& difficulty) const;
		void ApplyTurnEffects() const;
	private:
		int worldId;
	};
}
