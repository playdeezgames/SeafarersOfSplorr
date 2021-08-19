#pragma once
#include "Common.XY.h"
#include "Game.Difficulty.h"
#include "Game.Feature.h"
namespace game::islands::Features
{
	bool Read(const common::XY<double>&, const game::Feature&);
	void Write(const common::XY<double>&, const game::Feature&, bool);
	void Reset(const game::Difficulty&);
}