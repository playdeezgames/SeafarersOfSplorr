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
		std::list<Item> GetItems(const item::Type&) const;
		int GetItemQuantity(const item::Type&) const;
		void AddItemQuantity(const item::Type&, int) const;
		void RemoveItemQuantity(const item::Type&, int) const;
	private:
		int characterId;
	};
}