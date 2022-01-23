#pragma once
#include "Game.Session.Island.h"
#include <optional>
namespace game::session::ship
{
	struct DockableIslands
	{
		constexpr explicit DockableIslands(int shipId) 
			: shipId(shipId) {}
		std::optional<Island> TryGetFirst() const;
		std::optional<int> TryGetFirstId() const;
	private:
		int shipId;
	};
}