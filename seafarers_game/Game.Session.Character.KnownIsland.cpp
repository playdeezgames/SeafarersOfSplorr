#include <Data.Game.Character.Island.Known.h>
#include <Data.Game.Island.h>
#include "Game.Session.Character.KnownIsland.h"
namespace game::session::character
{
	bool KnownIsland::IsKnown() const
	{
		return data::game::character::island::Known::Has(characterId, islandId);
	}

	static const std::string UNKNOWN_ISLAND_NAME = "????";

	std::string KnownIsland::GetDisplayName() const
	{
		return 
			(IsKnown()) ? (data::game::Island::ReadName(islandId).value()) : 
			(UNKNOWN_ISLAND_NAME);
	}
}