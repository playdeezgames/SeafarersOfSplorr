#pragma once
#include "Game.Session.Ship.Berth.h"
#include <list>
namespace game::session::ship
{
	struct Berths
	{
		constexpr explicit Berths(int shipId) : shipId(shipId) {}
		std::list<Berth> GetBerths() const;
	private:
		int shipId;
	};
}
