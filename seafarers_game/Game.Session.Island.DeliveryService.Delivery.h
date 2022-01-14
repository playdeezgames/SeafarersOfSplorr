#pragma once
namespace game::session::island::delivery_service
{
	struct Delivery
	{
		constexpr explicit Delivery(int deliveryId) : deliveryId(deliveryId) {}
		constexpr explicit operator int() const { return deliveryId; }
		int GetToIslandId() const;
	private:
		int deliveryId;
	};
}