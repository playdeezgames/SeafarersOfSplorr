#pragma once
#include <optional>
namespace game
{
	struct Player
	{
		static void Create(int);
		static std::optional<int> TryGetCharacterId();
		static int GetCharacterId();
	};
}
