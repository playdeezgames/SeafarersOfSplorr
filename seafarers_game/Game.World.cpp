#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.RNG.h>
#include <Data.Game.World.h>
#include "Game.World.h"
#include <map>
namespace game
{
	static const int CURRENT_VERSION = 1;

	static data::game::World Read()
	{
		return data::game::World::Read().value();
	}

	common::XY<double> World::GetSize()
	{
		return Read().size;
	}

	int World::GetVersion()
	{
		return Read().version;
	}

	double World::GetMinimumIslandDistance()
	{
		return Read().minimumIslandDistance;
	}

	double World::GetViewDistance()
	{
		return Read().viewDistance;
	}

	double World::GetDockDistance()
	{
		return Read().dockDistance;
	}

	double World::GetUnfoulingLaborMultiplier()
	{
		return Read().unfoulingLaborMultiplier;
	}

	bool World::ClampLocation(common::XY<double>& location)
	{
		bool result = false;
		auto worldSize = game::World::GetSize();
		if (location.GetX() < 0.0)
		{
			result = true;
			location = { 0, location.GetY() };
		}
		if (location.GetX() > worldSize.GetX())
		{
			result = true;
			location = { worldSize.GetX(), location.GetY() };
		}
		if (location.GetY() < 0.0)
		{
			result = true;
			location = { location.GetX(), 0.0 };
		}
		if (location.GetY() > worldSize.GetY())
		{
			result = true;
			location = { location.GetX(), worldSize.GetY() };
		}
		return result;
	}

	double World::GetReputationReward()
	{
		return 1.0;
	}

	double World::GetReputationPenalty()
	{
		return -1.0;
	}

}