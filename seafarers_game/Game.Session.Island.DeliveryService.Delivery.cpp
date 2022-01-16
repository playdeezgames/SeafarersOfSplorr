#include <Data.Game.Delivery.h>
#include "Game.Session.Island.DeliveryService.Delivery.h"
namespace game::session::island::delivery_service
{
	int Delivery::GetToIslandId() const
	{
		return data::game::Delivery::ReadToIsland(deliveryId).value();
	}

	int Delivery::GetFromIslandId() const
	{
		return data::game::Delivery::ReadFromIsland(deliveryId).value();
	}

	int Delivery::GetTimeLimit() const
	{
		return data::game::Delivery::ReadTimeLimit(deliveryId).value();
	}

}