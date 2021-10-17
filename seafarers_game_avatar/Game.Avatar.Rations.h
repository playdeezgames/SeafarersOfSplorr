#pragma once
#include "Game.Item.h"
#include <optional>
namespace game::avatar//20211017
{
	struct Rations
	{
		static std::optional<Item> Read();
		static void Write(const std::optional<Item>&);
	};
}
