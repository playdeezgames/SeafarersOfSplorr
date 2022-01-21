#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Character.h"
#include <map>
#include <optional>
namespace game::session
{
	struct Player
	{
		constexpr Player() {}
		void Reset() const;
		void Populate(const Difficulty& difficulty, const std::map<int, size_t> allocations) const;
		Character GetCharacter() const;
		std::optional<Character> TryGetCharacter() const;
	};
}
