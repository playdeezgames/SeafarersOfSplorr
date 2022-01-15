#include <Data.Game.Delivery.h>
#include "Game.Session.Character.Delivery.h"
namespace game::session::character
{
	int Delivery::GetToIslandId() const
	{
		return data::game::Delivery::ReadToIsland(deliveryId).value();
	}

	int Delivery::GetFromIslandId() const
	{
		return data::game::Delivery::ReadFromIsland(deliveryId).value();
	}
}