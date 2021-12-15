#pragma once
#include "Game.Session.Ship.h"
namespace game::session
{
	struct Ships
	{
		Ship GetShip(int) const;
	};
}
