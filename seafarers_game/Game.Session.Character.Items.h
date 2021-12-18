#pragma once
#include "Game.Session.Item.h"
#include <list>
namespace game::session::character
{
	struct Items
	{
		constexpr Items(int characterId): characterId(characterId) {}
		std::list<Item> GetAll() const;
		void Purge() const;
		void Add(const Item&) const;
	private:
		int characterId;
	};
}