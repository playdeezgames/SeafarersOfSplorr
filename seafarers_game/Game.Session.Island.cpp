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

	std::string Island::GetName() const
	{
		return data::game::Island::ReadName(islandId).value();
	}

	int Island::GetPatronDemigodId() const
	{
		return data::game::Island::ReadPatronDemigodId(islandId).value();
	}

}