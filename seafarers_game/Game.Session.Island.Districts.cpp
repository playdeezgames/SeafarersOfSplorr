#include <algorithm>
#include <Data.Game.Island.District.h>
#include <Data.Game.Island.Feature.h>
#include <functional>
#include "Game.Island.FeatureType.h"
#include "Game.Session.Island.Districts.h"
#include <iterator>
#include <map>
#include <vector>
namespace game::session::island
{
	std::vector<District> Districts::GetDistricts() const
	{
		std::vector<District> result;
		auto districts = data::game::island::District::Read(islandId);
		std::transform(
			districts.begin(),
			districts.end(),
			std::back_inserter(result),
			[this](int district) 
			{
				return District(islandId,(game::island::District)district);
			});
		return result;
	}

	struct FeatureGenerator
	{
		std::function<int()> generateCount;
		std::function<std::string()> generateName;
		game::island::FeatureType featureType;
		std::function<game::island::District()> generateDistrict;
		std::function<void(int)> generateDetails;
	};

	static const std::vector<FeatureGenerator> featureGenerators =
	{
		{
			[]() { return 1; },
			[]() { return "Delivery Service"; },
			game::island::FeatureType::DELIVERY_SERVICE,
			[]() { return game::island::District::BUSINESS; },
			[](int featureId) {}
		}
	};

	void Districts::Populate(const game::Difficulty& difficulty) const
	{
		std::for_each(
			featureGenerators.begin(),
			featureGenerators.end(),
			[this](const FeatureGenerator& generator) 
			{
				int count = generator.generateCount();
				while (count > 0)
				{
					count--;
					auto name = generator.generateName();
					auto district = generator.generateDistrict();
					auto featureId = 
						data::game::island::Feature::Create(islandId, name, (int)generator.featureType, (int)district);
					generator.generateDetails(featureId);
				}
			});
	}

}