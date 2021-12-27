#pragma once
#include "Game.Difficulty.h"
#include "Game.ItemLegacy.h"
#include "Game.OfferingResult.h"
#include <string>
namespace game
{
	struct Demigods
	{
		static OfferingResult MakeOffering(int, int, const Item&);
		static std::string ReadName(int);
	};
}