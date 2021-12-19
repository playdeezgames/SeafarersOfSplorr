#pragma once
#include "Game.Session.Item.h"
#include "Game.Session.ItemSubtype.h"
#include <list>
namespace game::session::character
{
	struct Items
	{
		constexpr Items(int characterId): characterId(characterId) {}
		std::list<Item> GetAll() const;
		void Purge() const;
		void Add(const Item&) const;
		std::list<Item> GetItems(const ItemSubtype&) const;
	private:
		int characterId;
	};
}