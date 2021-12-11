#pragma once
#include "Game.BerthType.h"
#include "Game.Session.Ship.h"
#include <optional>
namespace game::session::character
{
	struct Berth
	{
		Berth(int);
		std::optional<game::session::Ship> GetShip() const;
		std::optional<BerthType> GetBerthType() const;
	private:
		int characterId;
	};
}
