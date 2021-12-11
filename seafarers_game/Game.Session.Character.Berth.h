#pragma once
#include "Game.BerthType.h"
#include <optional>
namespace game::session::character
{
	struct Berth
	{
		Berth(int);
		std::optional<int> GetShipId() const;
		std::optional<BerthType> GetBerthType() const;
	private:
		int characterId;
	};
}
