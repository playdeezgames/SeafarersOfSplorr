#pragma once
#include "Game.Commodity.h"
namespace game::session::item::type
{
	struct Commodity
	{
		constexpr Commodity(int itemTypeId, const game::Commodity& commodity)
			: itemTypeId(itemTypeId), commodity(commodity) {}
		constexpr operator game::Commodity() const { return commodity; }
		void SetAmount(double amount) const;
		double GetAmount() const;
	private:
		int itemTypeId;
		game::Commodity commodity;
	};
}