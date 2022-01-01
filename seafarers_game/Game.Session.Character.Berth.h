#pragma once
#include "Game.BerthType.h"
#include "Game.Session.Ship.h"
#include <optional>
namespace game::session::character
{
	struct Berth
	{
		constexpr explicit Berth(int characterId) : characterId(characterId) {}
		game::session::Ship GetShip() const;
		BerthType GetBerthType() const;
	private:
		int characterId;
	};
}
