#include <Data.Game.Delivery.h>
#include "Game.Session.Delivery.h"
namespace game::session
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

	double Delivery::GetRewardValue() const
	{
		return data::game::Delivery::ReadRewardValue(deliveryId).value();
	}
}