#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Character.h"
namespace game::session
{
	struct Player
	{
		void Reset(const Difficulty&) const;
		Character GetCharacter() const;
	};
}
