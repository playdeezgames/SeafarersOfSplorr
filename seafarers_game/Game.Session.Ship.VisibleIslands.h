#pragma once
#include <vector>
namespace game::session::ship
{
	struct VisibleIslands
	{
		constexpr explicit VisibleIslands(int shipId) 
			: shipId(shipId) {}
		std::vector<int> GetIslandIds() const;
		bool HasAny() const;
		int GetCount() const;
	private:
		int shipId;
	};
}