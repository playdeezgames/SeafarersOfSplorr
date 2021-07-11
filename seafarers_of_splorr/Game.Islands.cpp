#include "Game.Islands.h"
#include "Game.World.h"
#include "Data.Game.Island.h"
#include "Common.RNG.h"
#include <set>
#include <map>
#include <sstream>
#include "Game.Avatar.h"
#include "Game.Heading.h"
#include "Data.Game.Island.Visits.h"
#include "Data.Game.Island.Known.h"
#include "Data.Game.Island.BoundFor.h"
namespace game::Islands
{
	const size_t RETRY_COUNT = 500;

	static std::list<common::XY<double>> GenerateLocations()
	{
		double minimumIslandDistance = game::World::GetMinimumIslandDistance();
		auto worldSize = game::World::GetSize();
		size_t retry = 0;
		std::list<common::XY<double>> locations;
		while (retry < RETRY_COUNT)
		{
			double x = common::RNG::FromRange(0.0, worldSize.GetX());
			double y = common::RNG::FromRange(0.0, worldSize.GetY());
			bool found = true;
			for (auto& location : locations)
			{
				double distance = game::Heading::Distance(location, {x, y});
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

	static std::string GenerateName()
	{
		auto length = common::RNG::FromGenerator(LENGTH_GENERATOR, (size_t)0);
		auto isVowel = common::RNG::FromGenerator(IS_VOWEL_GENERATOR, false);
		std::stringstream ss;
		while (length > 0)
		{
			if (isVowel)
			{
				ss << common::RNG::FromGenerator(VOWEL_GENERATOR, (std::string)"");
			}
			else
			{
				ss << common::RNG::FromGenerator(CONSONANT_GENERATOR, (std::string)"");
			}
			length--;
			isVowel = !isVowel;
		}
		return ss.str();;
	}

	static std::set<std::string> GenerateNames(size_t size)
	{
		std::set<std::string> names;
		while (names.size() < size)
		{
			names.insert(GenerateName());
		}
		return names;
	}

	static void ClearData()
	{
		data::game::Island::Clear();
		data::game::island::Visits::Clear();
		data::game::island::Known::Clear();
		data::game::island::BoundFor::Clear();
	}

	void Reset(const game::Difficulty&)
	{
		ClearData();

		auto locations = GenerateLocations();
		auto names = GenerateNames(locations.size());
		while (!locations.empty())
		{
			data::game::Island::IslandData data =
			{
				locations.front(),
				*names.begin()
			};
			data::game::Island::Write(data);
			locations.pop_front();
			names.erase(names.begin());
		}
	}

	static std::list<IslandModel> GetIslandsInRange(double maximumDistance)
	{
		std::list<IslandModel> result;
		auto avatarLocation = game::Avatar::GetLocation();
		auto islands = data::game::Island::All();
		for (auto& island : islands)
		{
			auto distance = game::Heading::Distance(avatarLocation, island.location);
			if (distance <= maximumDistance)
			{
				auto visitData = data::game::island::Visits::Read(island.location);
				data::game::island::Known::Write(island.location);
				result.push_back(
					{
						(island.location - avatarLocation),
						island.location,
						island.name,
						(visitData.has_value()) ? (std::optional<int>(visitData.value().visits)) : (std::nullopt)
					});
			}
		}
		return result;
	}

	std::list<IslandModel> GetViewableIslands()
	{
		return GetIslandsInRange(game::World::GetViewDistance());
	}

	std::list<IslandModel> GetDockableIslands()
	{
		return GetIslandsInRange(game::World::GetDockDistance());
	}

	void AddVisit(const common::XY<double>& location, const int& turn)
	{
		auto data = data::game::island::Visits::Read(location);
		if (data)
		{
			auto islandVisits = data.value();
			if (islandVisits.lastVisit != turn)
			{
				islandVisits.visits = islandVisits.visits + 1;
				islandVisits.lastVisit = turn;
				data::game::island::Visits::Write(islandVisits);
				return;
			}
		}
		data::game::island::Visits::Write({
			location,
			1,
			turn});
	}

	std::optional<IslandModel> Read(const common::XY<double>& location)//TODO: different model for docked?
	{
		auto data = data::game::Island::Read(location);
		if (data)
		{
			auto visitData = data::game::island::Visits::Read(data.value().location);
			return std::optional<IslandModel>({
				{0.0, 0.0},
				data.value().location,
				data.value().name,
				(visitData.has_value()) ? (std::optional<int>(visitData.value().visits)) : (std::nullopt)
				});
		}
		return std::nullopt;
	}

	std::list<IslandModel> GetKnownIslands()
	{
		auto knownLocations = data::game::island::Known::All();
		std::list<IslandModel> result;
		for (auto& knownLocation : knownLocations)
		{
			auto model = Read(knownLocation);
			if (model)
			{
				auto visits = data::game::island::Visits::Read(knownLocation);
				if (visits)
				{
					result.push_back(model.value());
				}
				else
				{
					//TODO: obfuscate the name, because we havent visited
					result.push_back(model.value());
				}
			}
		}
		return result;
	}

}
