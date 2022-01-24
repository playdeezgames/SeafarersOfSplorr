#pragma once
namespace game::session
{
	struct Delivery
	{
		constexpr explicit Delivery(int deliveryId) : deliveryId(deliveryId) {}
		constexpr explicit operator int() const { return deliveryId; }
		int GetToIslandId() const;
		int GetFromIslandId() const;
		int GetTimeLimit() const;
		int GetRewardItemType() const;
		int GetRewardQuantity() const;
	private:
		int deliveryId;
	};
}
