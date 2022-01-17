#pragma once
#include "Game.Session.Delivery.h"
#include <vector>
namespace game::session::island::delivery_service
{
	struct Deliveries
	{
		constexpr explicit Deliveries(int featureId) : featureId(featureId) {}
		std::vector<Delivery> GetDeliveries() const;
	private:
		int featureId;
	};
}