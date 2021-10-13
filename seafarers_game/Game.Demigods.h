#pragma once
#include "Game.Difficulty.h"
#include "Game.Item.h"
#include "Game.OfferingResult.h"
#include <string>
namespace game//20211013
{
	struct Demigods
	{
		static void Reset(const Difficulty&);
		static OfferingResult MakeOffering(const std::string&, const Item&);
		static void ApplyTurnEffects();
	};
}