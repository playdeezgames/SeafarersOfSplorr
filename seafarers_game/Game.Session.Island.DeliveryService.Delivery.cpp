#include <Data.Game.Delivery.h>
#include "Game.Session.Island.DeliveryService.Delivery.h"
namespace game::session::island::delivery_service
{
	int Delivery::GetToIslandId() const
	{
		return data::game::Delivery::ReadToIsland(deliveryId).value();
	}

}