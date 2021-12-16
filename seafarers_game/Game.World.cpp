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

	double World::GetUnfoulingLaborMultiplier()
	{
		return Read().unfoulingLaborMultiplier;
	}
}