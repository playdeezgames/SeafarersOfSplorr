#pragma once
#include "Game.Session.Item.Type.h"
namespace game::session::island::delivery_service
{
	struct Delivery
	{
		constexpr explicit Delivery(int deliveryId) : deliveryId(deliveryId) {}
		constexpr explicit operator int() const { return deliveryId; }
		int GetToIslandId() const;
		int GetFromIslandId() const;
		int GetTimeLimit() const;
		item::Type GetRewardItemType() const;
		int GetRewardQuantity() const;
	private:
		int deliveryId;
	};
}