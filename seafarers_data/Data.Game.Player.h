#pragma once
#include <optional>
namespace data::game
{
	struct Player
	{
		Player() = delete;
		static void Initialize();
		static void Create(int characterId);
		static std::optional<int> GetCharacterId();
	};
}