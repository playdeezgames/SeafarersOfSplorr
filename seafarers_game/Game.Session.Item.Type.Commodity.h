#pragma once
#include "Game.Commodity.h"
namespace game::session::item::type
{
	struct Commodity
	{
		constexpr Commodity(int itemSubtypeId, const game::Commodity& commodity) : itemSubtypeId(itemSubtypeId), commodity(commodity) {}
		constexpr operator game::Commodity() const { return commodity; }
		void SetAmount(double) const;
		double GetAmount() const;
	private:
		int itemSubtypeId;
		game::Commodity commodity;
	};
}