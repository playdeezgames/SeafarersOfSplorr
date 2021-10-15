#include <Common.Utility.h>
#include "Game.Features.h"
#include <map>
namespace game
{
	struct FeatureDescriptor
	{
		std::string name;
		size_t minimumCount;//at least this many have to exist in a given world, up to the number of islands present
		double coveragePercentage;//the percentage of islands with this feature
	};

	static const std::map<game::Feature, game::FeatureDescriptor> featureDescriptors =
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
				0.6
			}
		}
	};
	static std::list<game::Feature> featureList;

	static const game::FeatureDescriptor& Read(const game::Feature& feature)
	{
		return featureDescriptors.find(feature)->second;
	}

	const std::list<game::Feature>& Features::All()
	{
		return common::Utility::ListFromTable(featureList, featureDescriptors);
	}

	const std::string& Features::GetName(const game::Feature& feature)
	{
		return Read(feature).name;
	}

	size_t Features::GetMinimumCount(const game::Feature& feature)
	{
		return Read(feature).minimumCount;
	}

	double Features::GetCoveragePercentage(const game::Feature& feature)
	{
		return Read(feature).coveragePercentage;
	}
}
