#pragma once
#include "Game.Difficulty.h"
#include "Game.Item.h"
#include "Game.OfferingResult.h"
#include <string>
namespace game
{
	struct Demigods
	{
		static void Reset(const Difficulty&);
		static OfferingResult MakeOffering(int, const Item&);
		static void ApplyTurnEffects();
		static std::string ReadName(int);
	};
}