#include <Common.Utility.Optional.h>
#include <Data.Game.Island.Tavern.h>
#include "Game.Islands.Taverns.h"
namespace game::islands
{
	std::optional<std::string> Taverns::GetName(int islandId)
	{
		return common::utility::Optional::Map<data::game::island::Tavern, std::string>(
			data::game::island::Tavern::Read(islandId),
			[](const data::game::island::Tavern& tavern) { return tavern.name; });
	}
}