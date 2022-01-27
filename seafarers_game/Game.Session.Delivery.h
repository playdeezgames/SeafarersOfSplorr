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
		double GetRewardValue() const;
	private:
		int deliveryId;
	};
}
