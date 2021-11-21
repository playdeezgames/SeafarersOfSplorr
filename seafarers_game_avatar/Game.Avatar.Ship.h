#pragma once
#include "Game.BerthType.h"
#include "Game.Difficulty.h"
#include "Game.ShipType.h"
#include <optional>
namespace game::avatar
{
	struct Ship
	{
		int shipId;
		BerthType berthType;//of the avatar

		static void Write(int, const Ship&);
		static std::optional<Ship> Read(int);
		static std::optional<int> ReadShipId(int);
		static std::optional<double> AvailableTonnage(int);
	};
}
