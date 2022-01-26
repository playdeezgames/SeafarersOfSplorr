#pragma once
#include "Game.Session.Character.ItemType.h"
#include <vector>
namespace game::session::character
{
	struct ItemTypes
	{
		constexpr explicit ItemTypes(int characterId) : characterId(characterId) {}
		constexpr ItemType GetItemType(int itemTypeId) { return ItemType(characterId, itemTypeId); }
		std::vector<ItemType> GetItemTypes() const;
		void AddItemTypeQuantity(int itemType, int delta) const;
		void RemoveItemTypeQuantity(int itemType, int delta) const;
		bool HasAny() const;
	private:
		int characterId;
	};
}
