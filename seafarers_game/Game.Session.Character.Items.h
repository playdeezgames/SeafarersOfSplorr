#pragma once
#include "Game.Session.Item.h"
#include <list>
namespace game::session::character
{
	struct Items
	{
		Items(int);
		std::list<Item> GetAll() const;
		void Purge() const;
		void Add(const Item&) const;
	private:
		int characterId;
	};
}