#pragma once
#include "Game.BerthType.h"
#include "Game.Session.Ship.h"
#include <optional>
namespace game::session::character
{
	struct Berth
	{
		Berth(int);
		game::session::Ship GetShip() const;
		BerthType GetBerthType() const;
	private:
		int characterId;
	};
}
