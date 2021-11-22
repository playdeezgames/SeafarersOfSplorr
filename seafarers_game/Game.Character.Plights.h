#pragma once
#include "Game.Character.Plight.h"
#include "Game.Character.PlightType.h"
#include "Game.Difficulty.h"
#include <optional>
#include <set>
#include <string>
namespace game::character
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
