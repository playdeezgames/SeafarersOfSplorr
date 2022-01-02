#include <Data.Game.Character.KnownIsland.h>
#include <Data.Game.Island.h>
#include "Game.Session.Character.KnownIsland.h"
#include <optional>
namespace game::session::character
{
	bool KnownIsland::IsKnown() const
	{
		return data::game::character::KnownIsland::Has(characterId, islandId);
	}

	static const std::string UNKNOWN_ISLAND_NAME = "????";

	std::string KnownIsland::GetDisplayName() const
	{
		return 
			(IsKnown()) ? (data::game::Island::ReadName(islandId).value()) : 
			(UNKNOWN_ISLAND_NAME);
	}
}