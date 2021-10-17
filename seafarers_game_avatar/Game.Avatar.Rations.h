#pragma once
#include "Game.Item.h"
#include <optional>
namespace game::avatar
{
	struct Rations
	{
		static std::optional<Item> Read();
		static void Write(const std::optional<Item>&);
	};
}
