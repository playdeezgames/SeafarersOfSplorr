#pragma once
#include "Game.Characters.Plight.h"
#include "Game.Characters.PlightType.h"
#include "Game.Difficulty.h"
#include <set>
#include <string>
namespace game::characters
{
	struct Plights
	{
		static Plight Generate(const PlightType&);
		static void Inflict(int, const Plight&);
		static std::set<Plight> InflictedWith(int);
		static void Reset();
	};
}
