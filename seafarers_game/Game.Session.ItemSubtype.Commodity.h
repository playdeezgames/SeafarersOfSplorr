#pragma once
#include "Game.Commodity.h"
namespace game::session::item_subtype
{
	struct Commodity
	{
		constexpr Commodity(int itemSubtypeId, const game::Commodity& commodity) : itemSubtypeId(itemSubtypeId), commodity(commodity) {}
		void SetAmount(double) const;
	private:
		int itemSubtypeId;
		game::Commodity commodity;
	};
}