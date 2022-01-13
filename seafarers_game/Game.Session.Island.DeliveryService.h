#pragma once
#include "Game.Difficulty.h"
namespace game::session::island
{
	struct DeliveryService
	{
		constexpr explicit DeliveryService(int featureId) : featureId(featureId) {}
		void ApplyTurnEffects() const;
		void Populate(const game::Difficulty& difficulty) const;
	private:
		int featureId;
	};
}
