#pragma once
#include "Game.Difficulty.h"
namespace game::session::island
{
	struct StreetVendor
	{
		constexpr StreetVendor(int featureId) : featureId(featureId) {}
		void Populate(const game::Difficulty& difficulty) const;
		void ApplyTurnEffects() const;
	private:
		int featureId;
	};
}