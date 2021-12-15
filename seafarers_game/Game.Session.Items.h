#pragma once
#include "Game.ItemType.h"
#include "Game.Session.Item.h"
namespace game::session
{
	struct Items
	{
		Item GetItem(int) const;
		void Purge() const;
		Item Add(int, int) const;
	};
}