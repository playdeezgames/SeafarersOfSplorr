#pragma once
#include "Game.Difficulty.h"
#include "Game.Fishery.h"
#include <list>
#include <optional>
namespace game
{
	struct Fisheries
	{
		static void Reset(const Difficulty&);
		static void ApplyTurnEffects();
		static std::list<Fishery> All();
		static std::list<Fishery> Available(int);
		static std::optional<Fishery> Read(int);
	};
}