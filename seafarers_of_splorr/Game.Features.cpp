#include "Game.Features.h"
#include <map>
namespace game::Features
{
	const std::map<game::Feature, game::FeatureDescriptor> featureDescriptors =
	{
		{game::Feature::SHIPYARD,
			{
				"shipyard",
				1,
				0.5
			}
		},
		{game::Feature::DARK_ALLEY,
			{
				"dark alley",
				1,
				1.0 //TODO: should be 0.6
			}
		}
	};
	std::list<game::Feature> featureList;

	const game::FeatureDescriptor& Read(const game::Feature& feature)
	{
		return featureDescriptors.find(feature)->second;
	}

	const std::list<game::Feature>& All()
	{
		if (featureList.empty())
		{
			for (auto entry : featureDescriptors)
			{
				featureList.push_back(entry.first);
			}
		}
		return featureList;
	}
}
