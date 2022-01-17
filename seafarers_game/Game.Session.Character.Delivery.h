#pragma once
#include "Game.Session.Delivery.h"
namespace game::session::character
{
	struct Delivery: public game::session::Delivery
	{
		constexpr explicit Delivery(int deliveryId) : game::session::Delivery(deliveryId) {}
		bool IsLate() const;
		void Abandon() const;
		void Complete() const;
		void ApplyTurnEffects() const;
	};
}
