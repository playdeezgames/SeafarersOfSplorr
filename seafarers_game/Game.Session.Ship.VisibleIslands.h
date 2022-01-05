#pragma once
#include "Game.Session.Island.h"
#include <vector>
namespace game::session::ship
{
	struct VisibleIslands
	{
		constexpr explicit VisibleIslands(int shipId) 
			: shipId(shipId) {}
		std::vector<Island> GetAll() const;
		bool HasAny() const;
		int GetCount() const;
	private:
		int shipId;
	};
}