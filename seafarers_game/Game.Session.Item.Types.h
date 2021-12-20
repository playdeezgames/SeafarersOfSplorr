#pragma once
#include "Game.Difficulty.h"
#include "Game.Item.Category.h"
#include "Game.Session.Item.Type.h"
#include <string>
namespace game::session
{
	struct ItemSubtypes
	{
		ItemSubtype GetItemSubtype(int) const;
		void Reset(const game::Difficulty&) const;
		ItemSubtype Create(const game::item::Category&, const std::string&) const;
	};
}