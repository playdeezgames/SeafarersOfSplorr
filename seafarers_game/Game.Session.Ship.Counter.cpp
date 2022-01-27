#include "Game.Session.Ship.Counter.h"
#include <Data.Game.Ship.Counter.h>
namespace game::session::ship
{
	int Counter::GetValue() const
	{
		return data::game::ship::Counter::Read(shipId, (int)counter).value_or(0);
	}
}