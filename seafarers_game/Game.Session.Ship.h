#pragma once
#include "Game.Session.Ship.Berth.h"
#include <list>
namespace game::session
{
	struct Ship
	{
		Ship(int);
		std::list<ship::Berth> GetBerths() const;
	private:
		int shipId;
	};
}
