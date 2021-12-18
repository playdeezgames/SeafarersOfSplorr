#pragma once
#include "Game.Session.ItemSubtype.h"
namespace game::session
{
	struct ItemTypes
	{
		ItemType GetItemType(int) const;
	};
}