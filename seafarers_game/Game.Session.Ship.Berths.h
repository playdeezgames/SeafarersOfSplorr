#pragma once
#include "Game.Session.Ship.Berth.h"
#include "Game.BerthType.h"
#include <vector>
namespace game::session::ship
{
	struct Berths
	{
		constexpr explicit Berths(int shipId) : shipId(shipId) {}
		std::vector<Berth> GetBerths() const;
		std::vector<Berth> GetBerths(const game::BerthType& berthType) const;
		bool HasAny(const game::BerthType& berthType) const;
	private:
		int shipId;
	};
}
