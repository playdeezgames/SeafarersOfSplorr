#include <Data.Game.Island.Feature.h>
#include <functional>
#include "Game.Session.Island.Feature.h"
#include <map>
#include "Game.Session.Island.DeliveryService.h"
#include "Game.Session.Island.StreetVendor.h"
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
			[](const auto& feature) { DeliveryService(feature.operator int()).ApplyTurnEffects();  }
		},
		{
			game::island::FeatureType::STREET_VENDOR,
			[](const auto& feature) { StreetVendor(feature.operator int()).ApplyTurnEffects();  }
		}
	};

	void Feature::ApplyTurnEffects() const
	{
		auto featureType = GetFeatureType();
		applyTurnEffectsForFeatureType.find(featureType)->second(*this);
	}

	static const std::map<game::island::FeatureType, std::function<void(const Feature&, const game::Difficulty&)>> populateForFeatureType =
	{
		{
			game::island::FeatureType::DELIVERY_SERVICE,
			[](const auto& feature, const game::Difficulty& difficulty) { DeliveryService(feature.operator int()).Populate(difficulty);  }
		},
		{
			game::island::FeatureType::STREET_VENDOR,
			[](const auto& feature, const game::Difficulty& difficulty) { StreetVendor(feature.operator int()).Populate(difficulty);  }
		}
	};

	void Feature::Populate(const game::Difficulty& difficulty) const
	{
		auto featureType = GetFeatureType();
		populateForFeatureType.find(featureType)->second(*this, difficulty);

	}

}