#pragma once
#include "Game.Difficulty.h"
#include "Game.ItemType.h"
#include "Game.Session.ItemSubtype.h"
namespace game::session
{
	struct ItemSubtypes
	{
		ItemSubtype GetItemSubtype(int) const;
		void Reset(const game::Difficulty&) const;
		ItemSubtype Create(const game::ItemType&) const;
	};
}