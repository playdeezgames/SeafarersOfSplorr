#pragma once
#include "Game.Difficulty.h"
#include "Game.ItemLegacy.h"
#include "Game.OfferingResult.h"
#include <string>
namespace game
{
	struct Demigods
	{
		static void Reset();
		static void Populate(const Difficulty&);
		static OfferingResult MakeOffering(int, int, const Item&);
		static void ApplyTurnEffects(int);
		static std::string ReadName(int);
	};
}