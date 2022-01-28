#include "Game.Session.Ship.Counter.h"
#include <Data.Game.Ship.Counter.h>
namespace game::session::ship
{
	using CounterData = data::game::ship::Counter;

	int Counter::GetValue() const
	{
		return CounterData::Read(shipId, (int)counter).value_or(0);
	}

	void Counter::ChangeBy(int delta) const
	{
		auto value = CounterData::Read(shipId, (int)counter).value_or(0) + delta;
		CounterData::Write(shipId, (int)counter, value);
	}

}