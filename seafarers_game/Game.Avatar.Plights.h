#pragma once
#include "Game.Avatar.Plight.h"
#include "Game.Avatar.PlightDescriptor.h"
namespace game::avatar
{
	struct Plights
	{
		static bool Has(const Plight&);
		static void ApplyTurnEffects();
		static const PlightDescriptor& Read(const Plight&);
	};
}
