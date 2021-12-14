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
	private:
		int characterId;
	};
}