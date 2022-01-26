#pragma once
namespace game::session::character
{
	struct ItemType
	{
		constexpr ItemType(int characterId, int itemTypeId) : characterId(characterId), itemTypeId(itemTypeId) {}
		constexpr int GetItemType() const { return itemTypeId; }
		int GetQuantity() const;
		bool HasAny() const { return GetQuantity() > 0; }
		void RemoveQuantity(int quantity) const;
	private:
		int characterId;
		int itemTypeId;
	};
}
