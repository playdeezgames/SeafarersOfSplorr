#pragma once
#include "Game.Ship.h"
#include "Game.ShipDescriptor.h"
#include <list>
namespace game
{
	struct Ships
	{
		static const game::ShipDescriptor& Read(const game::Ship&);//TODO: refactor me out!
		static const std::list<game::Ship>& All();
		static game::Ship GenerateForAvatar();
		static double GetAvailableTonnage(const game::Ship&);
	};
}
