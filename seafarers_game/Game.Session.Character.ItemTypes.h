#pragma once
#include "Game.Session.Character.ItemType.h"
namespace game::session::character
{
	struct ItemTypes
	{
		constexpr explicit ItemTypes(int characterId) : characterId(characterId) {}
		std::vector<ItemType> GetItemTypes() const;
		void AddItemTypeQuantity(const item::Type& itemType, int delta) const;
		void RemoveItemTypeQuantity(const item::Type&, int delta) const;//should this return how many were removed?
		bool HasAny() const;
	private:
		int characterId;
	};
}
