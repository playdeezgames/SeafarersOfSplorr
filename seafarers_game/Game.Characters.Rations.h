#pragma once
#include "Game.ItemLegacy.h"
#include <optional>
namespace game::characters
{
	struct Rations
	{
		static std::optional<Item> Read(int);
		static void Write(int, const std::optional<Item>&);
	};
}
