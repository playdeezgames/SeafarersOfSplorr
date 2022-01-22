#pragma once
#include "Game.Difficulty.h"
#include <map>
#include <optional>
namespace game::session
{
	struct Player
	{
		Player() = delete;
		static void Reset();
		static void Populate(const Difficulty& difficulty, const std::map<int, size_t> allocations);
		static int GetCharacterId();
		static std::optional<int> TryGetCharacterId();
	};
}
