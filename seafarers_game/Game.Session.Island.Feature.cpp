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

	static const std::map<game::island::FeatureType, std::function<void(const Feature&)>> applyTurnEffectsForFeatureType = 
	{
		{
			game::island::FeatureType::DELIVERY_SERVICE, 
			[](const auto& feature) { feature.GetDeliveryService().ApplyTurnEffects();  }
		}
	};

	void Feature::ApplyTurnEffects() const
	{
		auto featureType = GetFeatureType();
		applyTurnEffectsForFeatureType.find(featureType)->second(*this);
	}
}