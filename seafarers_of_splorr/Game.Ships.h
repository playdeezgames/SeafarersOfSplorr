#pragma once
#include "Game.Ship.h"
#include "Game.ShipDescriptor.h"
#include <list>
namespace game::Ships
{
	const game::ShipDescriptor& Read(const game::Ship&);
	const std::list<game::Ship>& All();
	game::Ship GenerateForAvatar();
}
