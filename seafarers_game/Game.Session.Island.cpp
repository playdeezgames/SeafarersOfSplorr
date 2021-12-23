#include <Data.Game.Island.h>
#include "Game.Islands.h"
#include "Game.Session.Island.h"
namespace game::session
{
	static std::optional<common::XY<double>> TryGetLocation(int islandId)
	{
		auto island = data::game::Island::Read(islandId);
		if (island)
		{
			return island.value().location;
		}
		return std::nullopt;
	}

	common::XY<double> Island::GetLocation() const
	{
		return TryGetLocation(islandId).value();
	}

	island::Tavern Island::GetTavern() const
	{
		return island::Tavern(islandId);
	}
}