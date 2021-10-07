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
		BerthType berthType;

		static void Reset(const game::Difficulty&);
		static void Write(const Ship&);
		static std::optional<Ship> Read();
		static std::optional<double> AvailableTonnage();
	};
}
