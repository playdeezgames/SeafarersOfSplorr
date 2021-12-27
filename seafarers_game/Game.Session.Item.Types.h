#pragma once
#include "Game.Difficulty.h"
#include "Game.Item.Category.h"
#include "Game.Session.Item.Type.h"
#include <string>
namespace game::session::item
{
	struct Types
	{
		Type GetItemSubtype(int) const;
		void Reset() const;
		Type Create(const game::item::Category&, const std::string&) const;
	};
}