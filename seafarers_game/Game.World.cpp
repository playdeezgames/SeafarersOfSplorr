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

	double World::GetReputationReward()
	{
		return 1.0;
	}

	double World::GetReputationPenalty()
	{
		return -1.0;
	}

}