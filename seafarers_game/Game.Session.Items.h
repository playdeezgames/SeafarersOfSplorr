#pragma once
#include "Game.Item.Category.h"
#include "Game.Session.Item.h"
namespace game::session
{
	struct Items
	{
		Item GetItem(int) const;
		void Purge() const;
		Item Add(const item::Type&, int) const;
	};
}