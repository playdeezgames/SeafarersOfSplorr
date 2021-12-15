#pragma once
#include <optional>
namespace data::game
{
	struct Player
	{
		static void Create(int);
		static std::optional<int> GetCharacterId();
	};
}