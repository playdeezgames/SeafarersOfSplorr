#pragma once
#include <Common.XY.h>
#include "Game.Difficulty.h"
#include "Game.Session.Demigod.h"
#include "Game.Session.Island.Markets.h"
#include <optional>
#include <string>
namespace game::session
{
	struct Island
	{
		constexpr Island(int islandId): islandId(islandId) {}
		constexpr operator int() const { return islandId; }
		constexpr island::Markets GetMarkets() const { return island::Markets(islandId); }
		common::XY<double> GetLocation() const;
		constexpr static Island ToIsland(int islandId) { return Island(islandId); }
		std::string GetName() const;
		Demigod GetPatronDemigod() const;
	private:
		int islandId;
	};
}
