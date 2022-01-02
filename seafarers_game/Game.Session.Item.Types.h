#pragma once
#include "Game.Difficulty.h"
#include "Game.Item.Category.h"
#include "Game.Session.Item.Type.h"
#include <string>
namespace game::session::item
{
	struct Types
	{
		constexpr Types() {}
		constexpr Type GetItemType(int itemTypeId) const { return Type(itemTypeId); }
		void Reset() const;
		Type Create(const game::item::Category& category, const std::string& name) const;
	};
}