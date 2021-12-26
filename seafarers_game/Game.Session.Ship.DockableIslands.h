#pragma once
#include "Game.Session.Island.h"
#include <list>
namespace game::session::ship
{
	struct DockableIslands
	{
		constexpr DockableIslands(int shipId) : shipId(shipId) {}
		std::list<Island> GetAll() const;
	private:
		int shipId;
	};
}