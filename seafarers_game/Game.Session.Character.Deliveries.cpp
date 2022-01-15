#include <algorithm>
#include <Data.Game.Character.Delivery.h>
#include "Game.Session.Character.Deliveries.h"
#include <iterator>
namespace game::session::character
{
	bool Deliveries::HasAny() const
	{
		return data::game::character::Delivery::ReadCount(characterId) > 0;
	}

	std::vector<Delivery> Deliveries::GetDeliveries() const
	{
		auto deliveries = data::game::character::Delivery::Read(characterId);
		std::vector<Delivery> result;
		std::transform(
			deliveries.begin(),
			deliveries.end(),
			std::back_inserter(result),
			[](int deliveryId) 
			{
				return Delivery(deliveryId);
			});
		return result;
	}

	bool Deliveries::HasDeliveriesFor(int islandId) const
	{
		auto deliveries = GetDeliveries();
		return std::any_of(
			deliveries.begin(), 
			deliveries.end(), 
			[=](const auto& delivery) 
			{
				return delivery.GetToIslandId() == islandId;
			});
	}

}