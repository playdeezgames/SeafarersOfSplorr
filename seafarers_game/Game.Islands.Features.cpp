#include <Common.RNG.h>
#include <Data.Game.Island.h>
#include <Data.Game.Island.DarkAlley.h>
#include <Data.Game.Island.Feature.h>
#include "Game.Features.h"
#include "Game.Islands.Features.h"
namespace game::islands
{
	bool Features::Read(int islandId, const game::Feature& feature)
	{
		return data::game::island::Feature::Read(islandId, (int)feature);
	}

	void Features::Write(int islandId, const game::Feature& feature, bool value)
	{
		if (value)
		{
			data::game::island::Feature::Write(islandId, (int)feature);
		}
		else
		{
			data::game::island::Feature::Clear(islandId, (int)feature);
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

	static void InitializeDarkAlley(int islandId)
	{
		data::game::island::DarkAlley::Write(islandId, 
			{ 
				GenerateInfamyRequirement(),
				GenerateRuffianBrawlingStrength(),
				GenerateMinimumWager() 
			});
	}

	static void InitializeTavern(int islandId)
	{
		//TODO: give the tavern a name
		//TODO: fill the tavern with NPC hirelings
	}

	static const std::map<game::Feature, std::function<void(int)>> featureInitializers =
	{
		{game::Feature::DARK_ALLEY, InitializeDarkAlley},
		{game::Feature::TAVERN, InitializeTavern}
	};

	static void InitializeFeature(const game::Feature& feature, int islandId)
	{
		auto initializer = featureInitializers.find(feature);
		if (initializer != featureInitializers.end())
		{
			initializer->second(islandId);
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
		std::vector<int> candidates;
		for (auto island : islands)
		{
			candidates.push_back(island.id);
		}
		size_t islandCount = DetermineIslandCountForFeature(feature, islands.size());
		while (islandCount > 0)
		{
			auto index = common::RNG::FromRange(0u, candidates.size());

			auto candidate = candidates[index];
			candidates[index] = candidates.back();
			candidates.pop_back();

			data::game::island::Feature::Write(candidate, (int)feature);
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