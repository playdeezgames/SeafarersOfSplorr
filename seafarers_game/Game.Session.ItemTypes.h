#pragma once
#include "Game.Session.ItemType.h"
namespace game::session
{
	struct ItemTypes
	{
		ItemType GetItemType(int) const;
	};
}