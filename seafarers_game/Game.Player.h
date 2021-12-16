#pragma once
#include <optional>
namespace game
{
	struct Player
	{
		static std::optional<int> TryGetCharacterId();
		static int GetCharacterId();
	};
}
