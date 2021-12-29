#pragma once
#include <Common.XY.h>
#include "Game.Difficulty.h"
#include "Game.Session.Island.Features.h"
#include "Game.Session.Island.Markets.h"
#include "Game.Session.Island.Tavern.h"
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
		constexpr island::Features GetFeatures() const { return island::Features(islandId); }
		constexpr island::Tavern GetTavern() const { return island::Tavern(islandId); }
		constexpr static Island ToIsland(int islandId) { return Island(islandId); }
		std::string GetName() const;
		int GetPatronDemigodId() const;
	private:
		int islandId;
	};
}
