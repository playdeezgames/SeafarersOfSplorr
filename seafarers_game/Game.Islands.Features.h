#pragma once
#include "Game.Difficulty.h"
#include "Game.Feature.h"
namespace game::islands
{
	struct Features
	{
		static bool Read(int, const game::Feature&);
		static void Write(int, const game::Feature&, bool);
		static void Reset(const game::Difficulty&);
		static void Populate(const game::Difficulty&);
	};
}