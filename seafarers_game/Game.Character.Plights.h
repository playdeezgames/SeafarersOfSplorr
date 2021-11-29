#pragma once
#include "Game.Character.Plight.h"
#include "Game.Character.PlightType.h"
#include "Game.Difficulty.h"
#include <set>
#include <string>
namespace game::character
{
	struct Plights
	{
		static bool Has(int, const Plight&);
		static void ApplyTurnEffects(int);
		static Plight Generate(const PlightType&);
		static void Inflict(int, const Plight&);
		static std::set<Plight> InflictedWith(int);
		static void Reset(const Difficulty&);
		static const std::string& GetName(const Plight&);
		static PlightType GetType(const Plight&);
	};
}
