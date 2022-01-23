#pragma once
#include <optional>
namespace game::session::ship
{
	struct DockableIslands
	{
		constexpr explicit DockableIslands(int shipId) 
			: shipId(shipId) {}
		std::optional<int> TryGetFirstId() const;
	private:
		int shipId;
	};
}