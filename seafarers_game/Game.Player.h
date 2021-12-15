#pragma once
#include <optional>
namespace game
{
	struct Player
	{
		static void CreateLegacy(int);
		static std::optional<int> TryGetCharacterId();
		static int GetCharacterId();
	};
}
