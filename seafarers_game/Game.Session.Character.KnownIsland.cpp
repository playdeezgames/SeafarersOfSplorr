#include "Game.Islands.h"
#include "Game.Session.Character.KnownIsland.h"
#include <optional>
namespace game::session::character
{
	bool KnownIsland::IsKnown() const
	{
		auto island = game::Islands::Read(characterId, islandId);
		if (island)
		{
			return island.value().isKnown;
		}
		return false;
	}

	std::string KnownIsland::GetDisplayName() const
	{
		auto island = game::Islands::Read(characterId, islandId);
		if (island)
		{
			return island.value().GetDisplayName();
		}
		return "????";
	}
}