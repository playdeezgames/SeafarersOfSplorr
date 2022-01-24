#pragma once
#include "Game.Difficulty.h"
#include "Game.Island.FeatureType.h"
#include <string>
namespace game::session::island
{
	struct Feature
	{
		constexpr explicit Feature(int featureId) : featureId(featureId) {}
		static constexpr Feature FromFeatureId(int featureId) { return Feature(featureId); }
		constexpr explicit operator int() const { return featureId; }
		std::string GetName() const;
		game::island::FeatureType GetFeatureType() const;
		void ApplyTurnEffects() const;
		void Populate(const game::Difficulty& difficulty) const;
	private:
		int featureId;
	};
}
