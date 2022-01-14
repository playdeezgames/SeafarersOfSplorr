#include <algorithm>
#include <Data.Game.Feature.Delivery.h>
#include "Game.Session.Island.DeliveryService.Deliveries.h"
#include <iterator>
namespace game::session::island::delivery_service
{
	std::vector<Delivery> Deliveries::GetDeliveries() const
	{
		auto deliveries = data::game::feature::Delivery::ReadForFeature(featureId);
		std::vector<Delivery> result{};
		std::transform(
			deliveries.begin(), 
			deliveries.end(), 
			std::back_inserter(result), 
			[](auto deliveryId) 
			{
				return Delivery(deliveryId);
			});
		return result;
	}
}