#include <Common.RNG.h>
#include <Data.Game.Island.h>
#include <Data.Game.Island.DarkAlley.h>
#include <Data.Game.Island.Feature.h>
#include "Game.Features.h"
#include "Game.Islands.Features.h"
#include "Game.Islands.Features.DarkAlley.h"
#include "Game.Islands.Features.Tavern.h"
namespace game::islands
{
	bool Features::Read(int islandId, const game::Feature& feature)
	{
		return data::game::island::Feature::Read(islandId, (int)feature);
	}

	void Features::Write(int islandId, const game::Feature& feature, bool value)
	{
		if (value)
		{
			data::game::island::Feature::Write(islandId, (int)feature);
		}
		else
		{
			data::game::island::Feature::Clear(islandId, (int)feature);
		}
	}
}