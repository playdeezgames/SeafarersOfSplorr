#include <Common.RNG.h>
#include <Data.Game.Island.h>
#include <Data.Game.Island.DarkAlley.h>
#include <Data.Game.Island.Feature.h>
#include <functional>
#include "Game.Features.h"
#include "Game.Islands.Features.h"
#include <vector>
namespace game::islands
{
	bool Features::Read(const common::XY<double>& xy, const game::Feature& feature)
	{
		return data::game::island::Feature::Read(data::game::Island::Find(xy).value(), (int)feature);
	}

	void Features::Write(const common::XY<double>& xy, const game::Feature& feature, bool value)
	{
		if (value)
		{
			data::game::island::Feature::Write(data::game::Island::Find(xy).value(), (int)feature);
		}
		else
		{
			data::game::island::Feature::Clear(data::game::Island::Find(xy).value(), (int)feature);
		}
	}

	static double GenerateInfamyRequirement()
	{
		return common::RNG::FromRange(1.0, 6.0) + common::RNG::FromRange(1.0, 6.0) + common::RNG::FromRange(1.0, 6.0);
	}

	static double GenerateRuffianBrawlingStrength()
	{
		return common::RNG::FromRange(1.0, 6.0) + common::RNG::FromRange(1.0, 6.0);
	}

	static double GenerateMinimumWager()
	{
		return common::RNG::FromRange(1.0, 4.0) + common::RNG::FromRange(1.0, 4.0);
	}

	static void InitializeDarkAlley(const common::XY<double>& location)
	{
		data::game::island::DarkAlley::Write(data::game::Island::Find(location).value(), {
			GenerateInfamyRequirement(),
			GenerateRuffianBrawlingStrength(),
			GenerateMinimumWager()
			});
	}

	static const std::map<game::Feature, std::function<void(const common::XY<double>&)>> featureInitializers =
	{
		{game::Feature::DARK_ALLEY, InitializeDarkAlley}
	};

	static void InitializeFeature(const game::Feature& feature, const common::XY<double>& location)
	{
		auto initializer = featureInitializers.find(feature);
		if (initializer != featureInitializers.end())
		{
			initializer->second(location);
		}
	}

	static size_t DetermineIslandCountForFeature(const game::Feature& feature, size_t totalIslandCount)
	{
		size_t islandCount = (size_t)(game::Features::GetCoveragePercentage(feature) * (double)totalIslandCount);
		if (islandCount < game::Features::GetMinimumCount(feature))
		{
			islandCount = game::Features::GetMinimumCount(feature);
		}
		if (islandCount > totalIslandCount)
		{
			islandCount = totalIslandCount;
		}
		return islandCount;
	}

	static void GenerateFeature(const game::Feature& feature, const std::list<data::game::Island>& islands)
	{
		std::vector<common::XY<double>> candidates;
		for (auto island : islands)
		{
			candidates.push_back(island.location);
		}
		size_t islandCount = DetermineIslandCountForFeature(feature, islands.size());
		while (islandCount > 0)
		{
			auto index = common::RNG::FromRange(0u, candidates.size());

			auto candidate = candidates[index];
			candidates[index] = candidates.back();
			candidates.pop_back();

			data::game::island::Feature::Write(data::game::Island::Find(candidate).value(), (int)feature);
			InitializeFeature(feature, candidate);
			islandCount--;
		}
	}

	static void GenerateAllFeatures()
	{
		auto allIslands = data::game::Island::All();
		for (auto feature : game::Features::All())
		{
			GenerateFeature(feature, allIslands);
		}
	}

	void Features::Reset(const game::Difficulty&)
	{
		data::game::island::Feature::Clear();
		GenerateAllFeatures();
	}
}