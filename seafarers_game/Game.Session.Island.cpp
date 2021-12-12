#include "Game.Islands.h"
#include "Game.Session.Island.h"
namespace game::session
{
	Island::Island(int islandId)
		: islandId(islandId)
	{

	}

	std::optional<bool> Island::IsKnown() const
	{
		auto island = game::Islands::Read(islandId);
		if (island)
		{
			return island.value().IsKnown();
		}
		return std::nullopt;
	}

	std::optional<std::string> Island::GetDisplayName() const
	{
		auto island = game::Islands::Read(islandId);
		if (island)
		{
			return island.value().GetDisplayName();
		}
		return std::nullopt;
	}
}