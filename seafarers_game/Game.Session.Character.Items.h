#pragma once
#include "Game.Session.Item.h"
#include "Game.Session.Item.Type.h"
#include <vector>
namespace game::session::character
{
	struct Items
	{
		constexpr explicit Items(int characterId): characterId(characterId) {}
		std::vector<Item> GetAll() const;
		std::vector<Item> GetItems(const item::Type& itemType) const;
		int GetItemQuantity(const item::Type& itemType) const;
		void AddItemQuantity(const item::Type& itemType, int delta) const;
		void RemoveItemQuantity(const item::Type&, int delta) const;//should this return how many were removed?
	private:
		int characterId;
	};
}