#pragma once
#include "Game.Session.Item.h"
namespace game::session
{
	struct Items
	{
		Item GetItem(int) const;
		void Purge() const;
	};
}