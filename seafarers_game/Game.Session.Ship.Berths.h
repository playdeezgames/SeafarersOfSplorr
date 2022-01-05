#pragma once
#include "Game.Session.Ship.Berth.h"
#include <vector>
namespace game::session::ship
{
	struct Berths
	{
		constexpr explicit Berths(int shipId) : shipId(shipId) {}
		std::vector<Berth> GetBerths() const;
	private:
		int shipId;
	};
}
