#pragma once
#include "Game.Ships.Counter.h"
namespace game::session::ship
{
	struct Counter
	{
		constexpr explicit Counter(int shipId, const game::ships::Counter& counter)
			: shipId(shipId), counter(counter) {}
		int GetValue() const;
	private:
		int shipId;
		game::ships::Counter counter;
	};
}