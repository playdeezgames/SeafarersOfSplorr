#pragma once
#include "Game.Commodity.h"
#include <string>
namespace game::session
{
	struct Commodity
	{
		constexpr explicit Commodity(const game::Commodity& commodity) : commodity(commodity) {}
		constexpr explicit operator game::Commodity() const { return commodity; }
		double GetBasePrice() const;
		double GetDemandFactor() const;
		double GetSupplyFactor() const;
		double GetDiscount() const;
		const std::string& GetName() const;
	private:
		game::Commodity commodity;
	};
}