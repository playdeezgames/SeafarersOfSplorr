#pragma once
#include "Game.Item.h"
namespace game::session::character
{
	struct Items
	{
		Items(int);
		bool Has(const game::Item&) const;
	private:
		int characterId;
	};
}