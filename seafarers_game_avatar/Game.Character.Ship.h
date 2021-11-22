#pragma once
#include "Game.BerthType.h"
#include "Game.Difficulty.h"
#include "Game.ShipType.h"
#include <optional>
namespace game::character
{
	struct Ship
	{
		static void Write(int, int, const BerthType&);
		static std::optional<int> ReadShipId(int);
		static std::optional<BerthType> ReadBerthType(int);
		static std::optional<double> AvailableTonnage(int);
	};
}
