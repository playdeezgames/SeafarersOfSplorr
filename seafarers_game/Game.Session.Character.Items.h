#pragma once
#include "Game.Session.Item.h"
#include "Game.Session.Item.Type.h"
#include <list>
namespace game::session::character
{
	struct Items
	{
		constexpr explicit Items(int characterId): characterId(characterId) {}
		std::list<Item> GetAll() const;
		std::list<Item> GetItems(const item::Type& itemType) const;
		int GetItemQuantity(const item::Type& itemType) const;
		void AddItemQuantity(const item::Type& itemType, int delta) const;
		void RemoveItemQuantity(const item::Type&, int delta) const;
	private:
		int characterId;
	};
}