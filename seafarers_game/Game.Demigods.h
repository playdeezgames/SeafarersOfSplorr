#pragma once
#include "Game.Difficulty.h"
#include "Game.Item.h"
#include "Game.OfferingResult.h"
#include <string>
namespace game
{
	struct Demigods
	{
		static void Reset(int, const Difficulty&);
		static OfferingResult MakeOffering(int, int, const Item&);
		static void ApplyTurnEffects(int);
		static std::string ReadName(int);
	};
}