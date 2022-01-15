#include <Data.Game.Character.Delivery.h>
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

	void Delivery::Abandon() const
	{
		//TODO: the stuff
		//give negative reputation at to island
		//give negative reputation at from island
		data::game::character::Delivery::Remove(deliveryId);
		data::game::Delivery::Remove(deliveryId);
	}
}