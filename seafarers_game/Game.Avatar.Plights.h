#pragma once
#include "Game.Avatar.Plight.h"
namespace game::avatar
{
	struct Plights
	{
		static bool Has(const Plight&);
		static void ApplyTurnEffects();
	};
}
