#pragma once
#include "Game.Session.Item.Type.h"
namespace game::session::character
{
	struct ItemType
	{
		constexpr ItemType(int characterId, int itemTypeId) : characterId(characterId), itemTypeId(itemTypeId) {}
		constexpr game::session::item::Type GetItemType() const { return game::session::item::Type(itemTypeId); }
		int GetQuantity() const;
	private:
		int characterId;
		int itemTypeId;
	};
}
