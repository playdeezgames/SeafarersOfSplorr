#include <algorithm>
#include <Data.Game.Island.Feature.h>
#include "Game.Session.Island.District.Features.h"
#include <iterator>
namespace game::session::island::district
{
	std::vector<Feature> Features::GetFeatures() const
	{
		auto features = data::game::island::Feature::ReadForIslandDistrict(islandId, (int)district);
		std::vector<Feature> result;
		std::transform(
			features.begin(), 
			features.end(), 
			std::back_inserter(result), 
			Feature::FromFeatureId);
		return result;
	}

	void Features::ApplyTurnEffects() const
	{
		auto features = GetFeatures();
		std::for_each(
			features.begin(),
			features.end(),
			[](const auto& feature) 
			{
				feature.ApplyTurnEffects();
			});
	}

	void Features::Populate(const game::Difficulty& difficulty) const
	{
		auto features = GetFeatures();
		std::for_each(
			features.begin(),
			features.end(),
			[difficulty](const auto& feature)
			{
				feature.Populate(difficulty);
			});
	}
}