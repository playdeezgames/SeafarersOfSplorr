#pragma once
namespace game::session::island
{
	struct DeliveryService
	{
		constexpr explicit DeliveryService(int featureId) : featureId(featureId) {}
		void ApplyTurnEffects() const;
	private:
		int featureId;
	};
}
