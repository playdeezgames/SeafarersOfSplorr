#include <Data.Game.Character.KnownIsland.h>
#include <Data.Game.Island.h>
#include "Game.Session.Character.KnownIsland.h"
#include <optional>
namespace game::session::character
{
	bool KnownIsland::IsKnown() const
	{
		return data::game::character::KnownIsland::Read(characterId, islandId);
	}

	std::string KnownIsland::GetDisplayName() const
	{
		if (IsKnown())
		{
			return data::game::Island::ReadName(islandId).value();
		}
		return "????";
	}
}