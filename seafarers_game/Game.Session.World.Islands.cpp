#include <algorithm>
#include <ranges>
#include <Common.Heading.h>
#include <Common.NameGenerator.h>
#include <Common.RNG.h>
#include <Data.Game.Island.h>
#include <Data.Game.Character.KnownIsland.h>
#include <Data.Game.Demigod.h>
#include <Data.Game.Island.Feature.h>
#include <Data.Game.Island.Market.h>
#include <Data.Game.Tribe.h>
#include <Data.Game.Island.Tribe.h>
#include "Game.Features.h"
#include "Game.Islands.Features.h"
#include "Game.Islands.Features.DarkAlley.h"
#include "Game.Islands.Features.Tavern.h"
#include "Game.Session.h"
#include "Game.Session.World.Islands.h"
#include <map>
#include <numeric>
#include <set>
#include <vector>
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

	static int GeneratePatronDemigod()
	{
		auto demigods = data::game::Demigod::All();
		std::map<int, size_t> table;
		for (auto& demigodId : demigods)
		{
			auto demigod = data::game::Demigod::Read(demigodId).value();
			table[demigodId] = demigod.patronWeight;
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

	static std::set<int> DetermineOccupiedIslands(
		const std::list<int>& tribes,
		const std::map<int, size_t>& islandGenerator,
		std::map<int, std::map<int, size_t>>& tribePresence
	)
	{
		constexpr int PRESENCE_DIE_SIZE = 6;
		std::set<int> occupiedIslands;
		std::for_each(
			tribes.begin(),
			tribes.end(),
			[
				&islandGenerator,
				&tribePresence,
				&occupiedIslands
			](int tribeId)
			{
				auto islandId = common::RNG::FromGenerator(islandGenerator);
				tribePresence[islandId][tribeId] = common::RNG::Roll<PRESENCE_DIE_SIZE>();
				occupiedIslands.insert(islandId);
			});
		return occupiedIslands;
	}

	static std::map<int, size_t> MakeIslandGenerator(const std::map<int, common::XY<double>>& islandLocations)
	{
		std::map<int, size_t> islandGenerator;
		std::transform(
			islandLocations.begin(),
			islandLocations.end(),
			std::inserter(islandGenerator, islandGenerator.end()),
			[](const std::pair<int, common::XY<double>> entry)
			{
				return std::make_pair(entry.first, 1);
			});
		return islandGenerator;
	}

	static std::map<int, std::map<int, size_t>> InitializeTribePresence(
		const std::map<int, common::XY<double>>& islandLocations)
	{
		std::map<int, std::map<int, size_t>> tribePresence;
		std::transform(
			islandLocations.begin(),
			islandLocations.end(),
			std::inserter(tribePresence, tribePresence.end()),
			[](const std::pair<int, common::XY<double>> entry)
			{
				return std::make_pair(entry.first, std::map<int, size_t>());
			});
		return tribePresence;
	}

	static std::map<int, size_t> DetermineIslandNeighbors(
		int islandId, 
		const std::map<int, common::XY<double>>& islandLocations)
	{
		const int TAKE_COUNT = 5;
		auto location = islandLocations.find(islandId)->second;
		std::vector<std::pair<int, common::XY<double>>> candidates;
		std::copy_if
		(
			islandLocations.begin(),
			islandLocations.end(),
			std::back_inserter(candidates),
			[islandId](const std::pair<int, common::XY<double>>& entry)
			{
				return entry.first != islandId;
			});
		std::partial_sort(
			candidates.begin(), 
			candidates.begin() + TAKE_COUNT,
			candidates.end(), 
			[location](
				const std::pair<int, common::XY<double>>& first, 
				const std::pair<int, common::XY<double>>& second)
			{
				auto firstDistance = (first.second - location).GetMagnitude();
				auto secondDistance = (second.second - location).GetMagnitude();
				return firstDistance < secondDistance;
			});
		std::map<int, size_t> result;
		std::for_each_n(
			candidates.begin(), 
			TAKE_COUNT, 
			[&result](const std::pair<int, common::XY<double>>& entry) 
			{ 
				result[entry.first] = 1;
			});
		return result;
	}

	static std::map<int, std::map<int, size_t>> DetermineIslandNeighbors(
		const std::map<int, common::XY<double>>& islandLocations)
	{
		std::map<int, std::map<int, size_t>> islandNeighbors;
		std::transform(
			islandLocations.begin(),
			islandLocations.end(),
			std::inserter(islandNeighbors, islandNeighbors.end()),
			[
				&islandLocations
			](const std::pair<int, common::XY<double>> entry)
			{
				return std::make_pair(entry.first, DetermineIslandNeighbors(entry.first, islandLocations));
			});
		return islandNeighbors;
	}

	static void PopulateIslandTribes()
	{
		constexpr int PILGRIM_DIE_SIZE = 6;
		constexpr int CHANCE_DIE_SIZE = 20;
		auto islandLocations = data::game::Island::AllLocations();
		auto tribes = data::game::Tribe::All();
		auto tribePresence = 
			InitializeTribePresence(islandLocations);
		auto islandGenerator = 
			MakeIslandGenerator(islandLocations);
		auto occupiedIslands = 
			DetermineOccupiedIslands(
				tribes, 
				islandGenerator, 
				tribePresence);
		std::map<int, std::map<int, size_t>> islandNeighbors = DetermineIslandNeighbors(islandLocations);
		while (occupiedIslands.size() < islandLocations.size())
		{
			std::set<int> addedIslands;
			std::set<int> removedIslands;
			std::for_each(
				occupiedIslands.begin(), 
				occupiedIslands.end(), 
				[
					&addedIslands, 
					&removedIslands,
					&tribePresence,
					&islandNeighbors
				](int islandId) 
				{
					auto tribe = common::RNG::TryFromGenerator(tribePresence[islandId]);
					if (tribe)
					{
						tribePresence[islandId][(int)tribe.value()] += (size_t)common::RNG::Roll<PILGRIM_DIE_SIZE>();
						auto presence = tribePresence[islandId][(int)tribe.value()];
						if (common::RNG::Roll<CHANCE_DIE_SIZE>() <= presence)
						{
							auto pilgrims = std::min((size_t)common::RNG::Roll<PILGRIM_DIE_SIZE>(), presence);
							tribePresence[islandId][(int)tribe.value()] = presence - pilgrims;
							auto destination = common::RNG::FromGenerator(islandNeighbors[islandId]);
							addedIslands.insert(destination);
							tribePresence[destination][(int)tribe.value()] += pilgrims;
						}
					}
					if (0 == std::accumulate(
						tribePresence[islandId].begin(),
						tribePresence[islandId].end(),
						(size_t)0,
						[](size_t acc, std::pair<int, size_t> item)
						{
							return acc + item.second;
						}))
					{
						removedIslands.insert(islandId);
					}
				});
			std::for_each(
				addedIslands.begin(), 
				addedIslands.end(), 
				[&occupiedIslands](int id) 
				{
					occupiedIslands.insert(id);
				});
			std::for_each(
				removedIslands.begin(),
				removedIslands.end(),
				[&occupiedIslands](int id) 
				{
					occupiedIslands.extract(id);
				});
		}
		std::for_each(
			tribePresence.begin(), 
			tribePresence.end(), 
			[](const std::pair<int, std::map<int, size_t>>& islandEntry) 
			{
				auto islandId = islandEntry.first;
				std::for_each(
					islandEntry.second.begin(),
					islandEntry.second.end(),
					[islandId](const std::pair<int, size_t>& tribeEntry)
					{
						if (tribeEntry.second > 0)
						{
							data::game::island::Tribe::Write(islandId, tribeEntry.first, (int)tribeEntry.second);
						}
					});
			});
	}

	void Islands::Populate(const Difficulty& difficulty) const
	{
		auto locations = GenerateLocations();
		auto names = GenerateNames(locations.size());
		while (!locations.empty())
		{
			auto islandId = 
				data::game::Island::Create(
					locations.front(), 
					*names.begin(), 
					GeneratePatronDemigod());
			locations.pop_front();
			names.erase(names.begin());
			GenerateMarkets(islandId);
		}
		PopulateIslandTribes();
		GenerateAllFeatures();
	}


	void Islands::Reset() const
	{
		data::game::Island::Clear();
		data::game::character::KnownIsland::Clear();
		data::game::island::Market::Clear();
		data::game::island::Feature::Clear();
	}

	void Islands::ApplyTurnEffects() const
	{

	}
}