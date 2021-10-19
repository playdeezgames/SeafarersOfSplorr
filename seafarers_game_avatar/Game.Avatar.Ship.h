#pragma once
#include "Game.BerthType.h"
#include "Game.Difficulty.h"
#include "Game.ShipType.h"
#include <optional>
namespace game::avatar//20211019
{
	struct Ship
	{
		int shipId;
		BerthType berthType;//of the avatar

		static void Reset(const game::Difficulty&);
		static void Write(const Ship&);
		static std::optional<Ship> Read();
		static std::optional<int> ReadShipId();
		static std::optional<double> AvailableTonnage();
	};
}
