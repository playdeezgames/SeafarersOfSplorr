#include <Data.Game.Island.Feature.h>
#include <functional>
#include "Game.Session.Island.Feature.h"
#include <map>
namespace game::session::island
{
	std::string Feature::GetName() const
	{
		return data::game::island::Feature::ReadName(featureId).value();
	}

	game::island::FeatureType Feature::GetFeatureType() const
	{
		return (game::island::FeatureType)data::game::island::Feature::ReadFeatureType(featureId).value();
	}

	static void ApplyTurnEffectsForDeliveryService(int featureId)
	{
		//TODO: update the delivery service for the turn
	}

	static const std::map<game::island::FeatureType, std::function<void(int)>> applyTurnEffectsForFeatureType = 
	{
		{game::island::FeatureType::DELIVERY_SERVICE, ApplyTurnEffectsForDeliveryService}
	};

	void Feature::ApplyTurnEffects() const
	{
		auto featureType = GetFeatureType();
		applyTurnEffectsForFeatureType.find(featureType)->second(featureId);
	}
}