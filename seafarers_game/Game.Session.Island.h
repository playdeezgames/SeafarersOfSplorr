#pragma once
#include <Common.XY.h>
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
		bool IsKnown(int characterId) const;
		std::string GetDisplayName(int characterId) const;
		common::XY<double> GetLocation() const;
		island::Tavern GetTavern() const;
	private:
		int islandId;
	};
}
