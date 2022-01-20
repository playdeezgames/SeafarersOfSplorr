#include <algorithm>
#include <ranges>
#include <Common.Heading.h>
#include <Common.NameGenerator.h>
#include <Common.RNG.h>
#include <Data.Game.Island.h>
#include <Data.Game.Character.Island.Known.h>
#include <Data.Game.Demigod.h>
#include <Data.Game.Island.Market.h>
#include <Data.Game.Tribe.h>
#include <Data.Game.Island.Tribe.h>
#include "Game.Session.h"
#include "Game.Session.World.h"
#include "Game.Session.World.Islands.h"
#include <iterator>
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
		double minimumIslandDistance = game::session::World().GetDistances().GetIslandSpacing();
		auto worldSize = game::session::World().GetBounds().GetSize();
		size_t retry = 0;
		std::list<common::XY<double>> locations;
		while (retry < RETRY_COUNT)
		{
			double x = common::RNG::FromRange(0.0, worldSize.GetX());
			double y = common::RNG::FromRange(0.0, worldSize.GetY());
			if (std::none_of(
				locations.begin(),
				locations.end(),
				[x, y, minimumIslandDistance](const common::XY<double>& location)
				{
					double distance = common::Heading::Distance(location, { x, y });
					return distance < minimumIslandDistance;
				}))
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
		std::for_each(
			permanentNames.begin(),
			permanentNames.end(),
			[&names](const std::string& name)
			{
				names.insert(name);
			});
		while (names.size() < size)
		{
			names.insert(GenerateName());
		}
		return names;
	}

	using Market = data::game::island::Market;


	static int GeneratePatronDemigod()
	{
		auto demigods = data::game::Demigod::All();
		std::map<int, size_t> table;
		std::transform(
			demigods.begin(), 
			demigods.end(), 
			std::inserter(table, table.end()), 
			[](int demigodId) 
			{
				return std::make_pair(demigodId, data::game::Demigod::ReadPatronWeight(demigodId).value());
			});
		return common::RNG::FromGenerator(table);
	}

	std::vector<Island> Islands::GetIslands() const
	{
		auto islands = IslandData::All();
		std::vector<Island> result;
		std::transform(
			islands.begin(),
			islands.end(),
			std::back_inserter(result),
			Island::ToIsland);
		return result;
	}

	static std::set<int> DetermineOccupiedIslands(
		const std::vector<int>& tribes,
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

	static void AttemptTribeColonization(
		std::set<int>& occupiedIslands,
		std::map<int, std::map<int, size_t>>& tribePresence,
		const std::map<int, std::map<int, size_t>>& islandNeighbors)
	{
		constexpr int PILGRIM_DIE_SIZE = 6;
		constexpr int CHANCE_DIE_SIZE = 20;
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
						auto colonists = std::min((size_t)common::RNG::Roll<PILGRIM_DIE_SIZE>(), presence);
						tribePresence[islandId][(int)tribe.value()] = presence - colonists;
						auto destination = common::RNG::FromGenerator(islandNeighbors.find(islandId)->second);
						addedIslands.insert(destination);
						tribePresence[destination][(int)tribe.value()] += colonists;
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

	static void StoreIslandTribes(const std::map<int, std::map<int, size_t>>& tribePresence)
	{
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
			AttemptTribeColonization(occupiedIslands, tribePresence, islandNeighbors);
		}
		StoreIslandTribes(tribePresence);
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
		}
		PopulateIslandTribes();
		auto allIslands = GetIslands();
		std::for_each(
			allIslands.begin(), 
			allIslands.end(), 
			[difficulty](const Island& island)
			{
				island.Populate(difficulty);
			});
	}

	void Islands::Reset() const
	{
		data::game::Island::Clear();
		data::game::character::island::Known::Clear();
		data::game::island::Market::Clear();
	}

	void Islands::ApplyTurnEffects() const
	{
		auto islands = GetIslands();
		std::for_each(
			islands.begin(), 
			islands.end(), 
			[](const auto& island) 
			{
				island.ApplyTurnEffects();
			});
	}
}