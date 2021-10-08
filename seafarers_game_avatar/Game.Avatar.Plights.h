#pragma once
#include "Game.Avatar.Plight.h"
#include "Game.Avatar.PlightType.h"
#include "Game.Difficulty.h"
#include <optional>
#include <set>
#include <string>
namespace game::avatar
{
	struct Plights
	{
		static bool Has(const Plight&);
		static void ApplyTurnEffects();
		static Plight Generate(const PlightType&);
		static void Inflict(const Plight&);
		static std::set<Plight> InflictedWith();
		static void Reset(const Difficulty&);
		static const std::string& GetName(const Plight&);
		static PlightType GetType(const Plight&);
	};
}
