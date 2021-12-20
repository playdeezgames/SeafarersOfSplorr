#pragma once
#include "Game.Session.Item.h"
#include "Game.Session.Item.Type.h"
#include <list>
namespace game::session::character
{
	struct Items
	{
		constexpr Items(int characterId): characterId(characterId) {}
		std::list<Item> GetAll() const;
		void Purge() const;
		std::list<Item> GetItems(const ItemSubtype&) const;
		int GetItemQuantity(const ItemSubtype&) const;
		void AddItemQuantity(const ItemSubtype&, int) const;
		void RemoveItemQuantity(const ItemSubtype&, int) const;
	private:
		int characterId;
	};
}