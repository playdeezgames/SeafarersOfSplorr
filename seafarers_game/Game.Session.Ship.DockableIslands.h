#pragma once
#include "Game.Session.Island.h"
#include <optional>
namespace game::session::ship
{
	struct DockableIslands
	{
		constexpr DockableIslands(int shipId) : shipId(shipId) {}
		std::optional<Island> TryGetFirst() const;
	private:
		int shipId;
	};
}