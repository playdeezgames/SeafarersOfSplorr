#pragma once
#include <Common.XY.h>
#include "Game.Difficulty.h"
#include <optional>
#include <string>
namespace game::session
{
	struct Island
	{
		constexpr Island(int islandId): islandId(islandId) {}
		constexpr operator int() const { return islandId; }
		constexpr static Island ToIsland(int islandId) { return Island(islandId); }

		double DistanceTo(const Island& otherIsland) const;
		double DistanceFrom(const common::XY<double>& location) const;
		common::XY<double> GetLocation() const;
		std::string GetName() const;
		int GetPatronDemigodId() const;
		void Populate(const game::Difficulty& difficulty) const;
		void ApplyTurnEffects() const;
	private:
		int islandId;
	};
}
