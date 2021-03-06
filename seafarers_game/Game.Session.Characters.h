#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Character.h"
#include <vector>
namespace game::session
{
	struct Characters
	{
		constexpr Characters() {}
		constexpr Character GetCharacter(int characterId) const { return Character(characterId); }
		constexpr void Populate(const Difficulty&) const {}

		std::vector<Character> GetAll() const;
		void Reset() const;
		void ApplyTurnEffects() const;
		Character Create();
	};
}
