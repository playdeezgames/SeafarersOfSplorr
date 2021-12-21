#pragma once
#include <string>
namespace game::session::island
{
	struct Tavern
	{
		constexpr Tavern(int islandId) : islandId(islandId) {}
		std::string GetName() const;
	private:
		int islandId;
	};
}
