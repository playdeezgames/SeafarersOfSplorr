#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Character.h"
#include <optional>
namespace game::session
{
	struct Player
	{
		void Reset() const;
		void Populate(const Difficulty& difficulty) const;
		Character GetCharacter() const;
		std::optional<Character> TryGetCharacter() const;
	};
}
