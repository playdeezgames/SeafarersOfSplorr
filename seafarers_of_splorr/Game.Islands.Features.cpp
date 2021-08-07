#include "Common.RNG.h"
#include "Data.Game.Island.h"
#include "Data.Game.Island.Feature.h"
#include "Game.Islands.Features.h"
#include "Game.Features.h"
#include <vector>
namespace game::islands::Features
{
	bool Read(const common::XY<double>& xy, const game::Feature& feature)
	{
		return data::game::island::Feature::Read(xy, (int)feature);
	}

	void Write(const common::XY<double>& xy, const game::Feature& feature, bool value)
	{
		if (value)
		{
			data::game::island::Feature::Write(xy, (int)feature);
		}
		else
		{
			data::game::island::Feature::Clear(xy, (int)feature);
		}
	}

	static void GenerateFeature(const game::Feature& feature, const std::list<data::game::Island::IslandData>& islands)
	{
		auto& descriptor = game::Features::Read(feature);
		std::vector<data::game::Island::IslandData> candidates;
		for (auto island : islands)
		{
			candidates.push_back(island);
		}
		size_t islandCount = (size_t)(descriptor.coveragePercentage * (double)islands.size());
		if (islandCount < descriptor.minimumCount)
		{
			islandCount = descriptor.minimumCount;
		}
		if (islandCount > islands.size())
		{
			islandCount = islands.size();
		}
		while (islandCount > 0)
		{
			auto index = common::RNG::FromRange(0u, candidates.size());
			auto candidate = candidates[index];
			candidates[index] = candidates.back();
			candidates.pop_back();
			data::game::island::Feature::Write(candidate.location, (int)feature);
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

	void Reset(const game::Difficulty&)
	{
		data::game::island::Feature::ClearAll();
		GenerateAllFeatures();
	}
}