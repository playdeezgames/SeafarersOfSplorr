#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Item.Type.h"
namespace game::session
{
	constexpr int WORLD_ID = 1;
	struct World
	{
		World() = delete;
		static item::Type GetCurrencyItemSubtype();
		static game::Difficulty GetDifficulty();
		static void Reset();
		static void Populate(const Difficulty& difficulty);
		static void ApplyTurnEffects();
	};
}
