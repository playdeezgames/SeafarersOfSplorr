#pragma once
#include "Game.Session.ItemSubtype.h"
namespace game::session
{
	struct ItemTypes
	{
		ItemSubtype GetItemSubtype(int) const;
	};
}