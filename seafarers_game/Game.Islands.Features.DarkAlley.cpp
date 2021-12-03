#include <Common.RNG.h>
#include <Data.Game.Island.DarkAlley.h>
#include "Game.Islands.Features.DarkAlley.h"
namespace game::islands::features
{
	static double GenerateInfamyRequirement()
	{
		return common::RNG::FromRange(1.0, 6.0) + common::RNG::FromRange(1.0, 6.0) + common::RNG::FromRange(1.0, 6.0);
	}

	static double GenerateRuffianBrawlingStrength()
	{
		return common::RNG::FromRange(1.0, 6.0) + common::RNG::FromRange(1.0, 6.0);
	}

	static double GenerateMinimumWager()
	{
		return common::RNG::FromRange(1.0, 4.0) + common::RNG::FromRange(1.0, 4.0);
	}

	void DarkAlley::Initialize(int islandId)
	{
		data::game::island::DarkAlley::Write(islandId,
			{
				GenerateInfamyRequirement(),
				GenerateRuffianBrawlingStrength(),
				GenerateMinimumWager()
			});
	}
}