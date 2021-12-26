#pragma once
#include "Game.Session.Island.h"
#include <list>
namespace game::session::ship
{
	struct VisibleIslands
	{
		constexpr VisibleIslands(int shipId) : shipId(shipId) {}
		std::list<Island> GetAll() const;
	private:
		int shipId;
	};
}