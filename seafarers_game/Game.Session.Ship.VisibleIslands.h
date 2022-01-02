#pragma once
#include "Game.Session.Island.h"
#include <list>
namespace game::session::ship
{
	struct VisibleIslands
	{
		constexpr explicit VisibleIslands(int shipId) : shipId(shipId) {}
		std::list<Island> GetAll() const;
		bool HasAny() const;
		int GetCount() const;
	private:
		int shipId;
	};
}