#pragma once
#include "Game.Characters.State.h"
#include "Game.Difficulty.h"
#include "Game.Session.Character.h"
#include <list>
namespace game::session
{
	struct Characters
	{
		Character GetCharacter(int) const;
		std::list<Character> GetAll() const;
		void Reset() const;
		void Populate(const Difficulty&) const {}
		void ApplyTurnEffects() const;
		Character Create(const game::characters::State&);
	};
}
