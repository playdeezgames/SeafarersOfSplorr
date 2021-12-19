#pragma once
#include <Common.XY.h>
#include "Game.Session.Island.Markets.h"
#include <optional>
#include <string>
namespace game::session
{
	struct Character;
	struct Island
	{
		constexpr Island(int islandId): islandId(islandId) {}
		constexpr operator int() const { return islandId; }
		constexpr island::Markets GetMarkets() const { return island::Markets(islandId); }
		bool IsKnown() const;
		std::string GetDisplayName() const;
		common::XY<double> GetLocation() const;
	private:
		int islandId;
	};
}
