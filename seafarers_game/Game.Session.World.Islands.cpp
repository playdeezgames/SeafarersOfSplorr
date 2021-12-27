#include <algorithm>
#include <ranges>
#include <Common.Heading.h>
#include <Common.NameGenerator.h>
#include <Common.RNG.h>
#include <Data.Game.Island.h>
#include <Data.Game.Character.KnownIsland.h>
#include <Data.Game.Demigod.h>
#include <Data.Game.Island.Feature.h>
#include <Data.Game.Island.ItemLegacy.h>
#include <Data.Game.Island.Market.h>
#include <Data.Game.Tribe.h>
#include <Data.Game.Island.Tribe.h>
#include "Game.Features.h"
#include "Game.Islands.h"
#include "Game.Islands.Features.h"
#include "Game.Islands.Features.DarkAlley.h"
#include "Game.Islands.Features.Tavern.h"
#include "Game.Items.h"
#include "Game.Session.h"
#include "Game.Session.World.Islands.h"
namespace game::session::world
{
	using IslandData = data::game::Island;

	const size_t RETRY_COUNT = 500;

	static std::list<common::XY<double>> GenerateLocations()
	{
		double minimumIslandDistance = game::Session().GetWorld().GetDistances().GetIslandSpacing();
		auto worldSize = game::Session().GetWorld().GetBounds().GetSize();
		size_t retry = 0;
		std::list<common::XY<double>> locations;
		while (retry < RETRY_COUNT)
		{
			double x = common::RNG::FromRange(0.0, worldSize.GetX());
			double y = common::RNG::FromRange(0.0, worldSize.GetY());
			bool found = true;
			for (auto& location : locations)
			{
				double distance = common::Heading::Distance(location, { x, y });
				if (distance < minimumIslandDistance)
				{
					found = false;
					break;
				}
			}
			if (found)
			{
				locations.push_back({ x, y });
				retry = 0;
			}
			else
			{
				retry++;
			}
		}
		return locations;
	}

	const std::map<std::string, size_t> CONSONANT_GENERATOR =
	{
		{"h", 1},
		{"k", 1},
		{"l", 1},
		{"m", 1},
		{"p", 1}
	};
	const std::map<std::string, size_t> VOWEL_GENERATOR =
	{
		{"a", 1},
		{"e", 1},
		{"i", 1},
		{"o", 1},
		{"u", 1}
	};
	const std::map<size_t, size_t> LENGTH_GENERATOR =
	{
		{3, 1},
		{4, 3},
		{5, 9},
		{6, 27},
		{7, 9},
		{8, 3},
		{9, 1}
	};
	const std::map<bool, size_t> IS_VOWEL_GENERATOR =
	{
		{true, 1},
		{false, 1}
	};
	static const common::NameGenerator nameGenerator =
	{
		LENGTH_GENERATOR,
		IS_VOWEL_GENERATOR,
		VOWEL_GENERATOR,
		CONSONANT_GENERATOR
	};

	static std::string GenerateName()
	{
		return nameGenerator.Generate();
	}

	const std::vector<std::string> permanentNames =
	{
		"antwerp"
	};

	static std::set<std::string> GenerateNames(size_t size)
	{
		std::set<std::string> names;
		for (auto& name : permanentNames)
		{
			names.insert(name);
		}
		while (names.size() < size)
		{
			names.insert(GenerateName());
		}
		return names;
	}

	static void GenerateMarkets(int islandId)
	{
		for (auto& commodity : game::Session().GetWorld().GetCommodities().GetAll())
		{
			data::game::island::Market data =
			{
				common::RNG::FromRange(1.0,6.0) + common::RNG::FromRange(1.0,6.0) + common::RNG::FromRange(1.0,6.0),
				common::RNG::FromRange(1.0,6.0) + common::RNG::FromRange(1.0,6.0) + common::RNG::FromRange(1.0,6.0),
				0,
				0
			};
			data::game::island::Market::Write(islandId, (int)commodity, data);
		}
	}

	static void GenerateItems(int islandId)
	{
		for (auto& item : game::Items::All())
		{
			if (game::Items::GeneratePresenceForIsland(item))
			{
				data::game::island::ItemLegacy::Write(islandId, (int)item);
			}
		}
	}

	static int GeneratePatronDemigod()
	{
		auto demigods = data::game::Demigod::All();
		std::map<int, size_t> table;
		for (auto& demigod : demigods)
		{
			table[demigod.id] = demigod.patronWeight;
		}
		return common::RNG::FromGenerator(table);
	}

	std::list<Island> Islands::GetAll() const
	{
		auto islands = IslandData::All();
		std::list<Island> result;
		std::transform(
			islands.begin(), 
			islands.end(), 
			std::back_inserter(result),
			Island::ToIsland);
		return result;
	}
	static const std::map<game::Feature, std::function<void(int)>> featureInitializers =
	{
		{game::Feature::DARK_ALLEY, game::islands::features::DarkAlley::Initialize},
		{game::Feature::TAVERN, game::islands::features::Tavern::Initialize}
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

	static void GenerateFeature(const game::Feature& feature, const std::list<int>& islands)
	{
		std::vector<int> candidates;
		for (auto island : islands)
		{
			candidates.push_back(island);
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

	static void PopulateIslandTribes()
	{
		auto tribes = data::game::Tribe().All();
		auto islands = data::game::Island::All();

	}

	void Islands::Populate(const Difficulty& difficulty) const
	{
		auto locations = GenerateLocations();
		auto names = GenerateNames(locations.size());
		while (!locations.empty())
		{
			data::game::Island data =
			{
				0,
				locations.front(),
				*names.begin(),
				GeneratePatronDemigod()
			};
			auto islandId = data::game::Island::Write(data);
			locations.pop_front();
			names.erase(names.begin());
			GenerateMarkets(islandId);
			GenerateItems(islandId);
		}
		GenerateAllFeatures();
		PopulateIslandTribes();
	}


	void Islands::Reset() const
	{
		data::game::Island::Clear();
		data::game::character::KnownIsland::Clear();
		data::game::island::Market::Clear();
		data::game::island::ItemLegacy::ClearAll();
		data::game::island::Feature::Clear();
	}

	void Islands::ApplyTurnEffects() const
	{

	}
}