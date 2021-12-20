#pragma once
#include "Game.ItemLegacy.h"
#include <string>
namespace game::session::character
{
	struct LegacyItem
	{
		LegacyItem(int, const game::Item&);
		std::string GetName() const;
		size_t GetCount() const;
	private:
		int characterId;
		game::Item item;
	};
}
