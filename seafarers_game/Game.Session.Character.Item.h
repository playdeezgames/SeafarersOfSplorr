#pragma once
#include "Game.Item.h"
#include <string>
namespace game::session::character
{
	struct Item
	{
		Item(int, const game::Item&);
		std::string GetName() const;
		size_t GetCount() const;
	private:
		int characterId;
		game::Item item;
	};
}
