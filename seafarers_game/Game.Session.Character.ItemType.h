#pragma once
namespace game::session::character
{
	struct ItemType
	{
		constexpr ItemType(int characterId, int itemTypeId) : characterId(characterId), itemTypeId(itemTypeId) {}
		constexpr int GetItemType() const { return itemTypeId; }
		int GetQuantity() const;
	private:
		int characterId;
		int itemTypeId;
	};
}
