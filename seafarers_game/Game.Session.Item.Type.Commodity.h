#pragma once
#include "Game.Commodity.h"
namespace game::session::item::type
{
	struct Commodity
	{
		constexpr Commodity(int typeId, const game::Commodity& commodity) : typeId(typeId), commodity(commodity) {}
		constexpr operator game::Commodity() const { return commodity; }
		void SetAmount(double) const;
		double GetAmount() const;
	private:
		int typeId;
		game::Commodity commodity;
	};
}