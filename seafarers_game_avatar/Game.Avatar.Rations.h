#pragma once
#include "Game.Item.h"
#include <optional>
namespace game::avatar//20211017
{
	struct Rations
	{
		static std::optional<Item> Read(int);
		static void Write(int, const std::optional<Item>&);
	};
}
