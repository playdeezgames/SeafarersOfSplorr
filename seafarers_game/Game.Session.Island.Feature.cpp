#include <Data.Game.Island.Feature.h>
#include "Game.Session.Island.Feature.h"
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
}