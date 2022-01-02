#pragma once
#include "Game.Characters.State.h"
#include "Game.Difficulty.h"
#include "Game.Session.Character.h"
#include <list>
namespace game::session
{
	struct Characters
	{
		constexpr Characters() {}
		constexpr Character GetCharacter(int characterId) const { return Character(characterId); }
		constexpr void Populate(const Difficulty& difficulty) const {}

		std::list<Character> GetAll() const;
		void Reset() const;
		void ApplyTurnEffects() const;
		Character Create(const game::characters::State& state);
	};
}
