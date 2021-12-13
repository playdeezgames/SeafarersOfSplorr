#pragma once
#include "Game.Side.h"
#include <optional>
namespace game::session::ship
{
	struct Fouling
	{
		Fouling(int, const Side&);
		double GetPercentage();
	private:
		int shipId;
		Side side;
	};
}
