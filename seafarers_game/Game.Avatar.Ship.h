#pragma once
#include "Game.Difficulty.h"
#include "Game.Ship.h"
namespace game::avatar::Ship
{
	void Reset(const game::Difficulty&);
	void Write(const game::Ship&);
	game::Ship Read();
	double AvailableTonnage();
}
