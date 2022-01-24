#pragma once
#include "Game.BerthType.h"
#include <optional>
namespace game::session::character
{
	struct Berth
	{
		constexpr explicit Berth(int characterId) : characterId(characterId) {}
		int GetShipId() const;
		BerthType GetBerthType() const;
	private:
		int characterId;
	};
}
