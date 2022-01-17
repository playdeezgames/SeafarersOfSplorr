#pragma once
#include "Game.Session.Item.Type.h"
namespace game::session::character
{
	struct Delivery
	{
		constexpr explicit Delivery(int deliveryId) : deliveryId(deliveryId) {}
		constexpr explicit operator int() const { return deliveryId; }//TODO: Base class
		int GetToIslandId() const;//TODO: Base class
		int GetFromIslandId() const;//TODO: Base class
		int GetTimeLimit() const;//TODO: Base class
		item::Type GetRewardItemType() const;//TODO: Base class
		int GetRewardQuantity() const;//TODO: Base class
		bool IsLate() const;
		void Abandon() const;
		void Complete() const;
		void ApplyTurnEffects() const;
	private:
		int deliveryId;
	};
}
