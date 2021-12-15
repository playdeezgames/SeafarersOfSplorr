#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Character.h"
namespace game::session
{
	struct Characters
	{
		Character GetCharacter(int) const;
		void Reset(const Difficulty&) const;
		void ApplyTurnEffects() const;
	};
}
