#pragma once
namespace game::session::character
{
	struct Delivery
	{
		constexpr explicit Delivery(int deliveryId) : deliveryId(deliveryId) {}
		constexpr explicit operator int() const { return deliveryId; }
		int GetToIslandId() const;
		int GetFromIslandId() const;
		void Abandon() const;
	private:
		int deliveryId;
	};
}