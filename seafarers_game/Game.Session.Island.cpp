#include <Data.Game.Island.h>
#include "Game.Islands.h"
#include "Game.Session.Island.h"
namespace game::session
{
	static std::optional<common::XY<double>> TryGetLocation(int islandId)
	{
		return data::game::Island::ReadLocation(islandId);
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