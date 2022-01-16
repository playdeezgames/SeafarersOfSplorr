#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.Island.DeliveryService.Deliveries.h"
namespace game::session::island
{
	struct DeliveryService
	{
		constexpr explicit DeliveryService(int featureId) : featureId(featureId) {}
		constexpr delivery_service::Deliveries GetDeliveries() const { return delivery_service::Deliveries(featureId); }

		bool HasDeliveries() const;
		void ApplyTurnEffects() const;
		void Populate(const game::Difficulty& difficulty) const;
	private:
		int featureId;
	};
}
