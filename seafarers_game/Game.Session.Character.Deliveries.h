#pragma once
#include "Game.Session.Character.Delivery.h"
#include <vector>
namespace game::session::character
{
	struct Deliveries
	{
		constexpr explicit Deliveries(int characterId) : characterId(characterId) {};
		bool HasAny() const;
		std::vector<Delivery> GetDeliveries() const;
	private:
		int characterId;
	};
}
