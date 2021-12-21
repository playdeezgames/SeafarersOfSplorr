#include <Common.Heading.h>
#include <Common.NameGenerator.h>
#include <Common.RNG.h>
#include <Data.Game.Demigod.h>
#include <Data.Game.Island.h>
#include <Data.Game.Island.ItemLegacy.h>
#include <Data.Game.Island.Known.h>
#include <Data.Game.Island.Market.h>
#include <Data.Game.Island.VisitLegacy.h>
#include "Game.Session.h"
#include "Game.Items.h"
#include <set>
#include <vector>
namespace game
{
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

	static void ClearData()
	{
		data::game::Island::Clear();
		data::game::island::Visit::Clear();
		data::game::island::Known::Clear();
		data::game::island::Market::Clear();
		data::game::island::Item::ClearAll();
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
			if (Items::GeneratePresenceForIsland(item))
			{
				data::game::island::Item::Write(islandId, (int)item);
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

	void GenerateIslands()
	{
		ClearData();
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
	}
}
