#pragma once
#include "Game.Difficulty.h"
#include "Game.Island.District.h"
#include "Game.Session.Island.Feature.h"
#include <vector>
namespace game::session::island::district
{
	struct Features
	{
		constexpr explicit Features(int islandId, const game::island::District& district) : islandId(islandId), district(district) {}
		std::vector<Feature> GetFeatures() const;
		void ApplyTurnEffects() const;
		void Populate(const game::Difficulty& difficulty) const;
	private:
		int islandId;
		game::island::District district;
	};
}
