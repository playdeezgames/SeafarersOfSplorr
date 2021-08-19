#include "Common.RNG.h"
#include "Data.Game.Island.h"
#include "Data.Game.Island.Quests.h"
#include <format>
#include "Game.Avatar.Statistics.h"
#include "Game.Islands.h"
#include "Game.Islands.Quests.h"
#include <map>
#include <string>
namespace game::islands::Quests
{
	const double MAXIMUM_REWARD = 10.0;
	static double GenerateReward()
	{
		double reputation = floor(game::avatar::Statistics::GetReputation());
		double minimum = 1.0;
		double maximum = 1.0;
		if (reputation <= 0.0)
		{
			minimum = std::pow(10.0, reputation);
		}
		else
		{
			maximum = std::min(MAXIMUM_REWARD, std::sqrt(reputation));
		}
		return common::RNG::FromRange(minimum, maximum);
	}

	const std::map<std::string, size_t> itemAdverbs = 
	{
		{"woefully",1},
		{"awfully",1},
		{"generally",1},
		{"executively",1},
		{"painfully",1},
		{"wickedly",1},
		{"nastily",1},
		{"evilly",1},
		{"gracefully",1},
		{"mercifully",1}
	};

	const std::map<std::string, size_t> itemAdjectives =
	{
		{"turgid",1},
		{"odorous",1},
		{"crusty",1},
		{"rusty",1},
		{"musty",1},
		{"dingy",1},
		{"dirty",1},
		{"purple",1},
		{"tatty",1},
		{"Belgian",1},
		{"throbbing",1},
		{"pulsating",1},
		{"writhing",1},
		{"bulging",1},
		{"chewy",1},
		{"swollen",1}
	};

	const std::map<std::string, size_t> itemNames =
	{
		{"marital aid",1},
		{"bed pan",1},
		{"skeet shooter",1},
		{"poo bucket",1},
		{"chastity belt",1},
		{"nipple clamp",1},
		{"beer bottle",1},
		{"wine glass",1},
		{"speculum",1},
		{"waffle maker",1},
		{"fleshlight",1},
		{"biscuit",1},
		{"marmite",1},
		{"vegemite",1},
		{"transfunctioner",1}
	};

	static std::string GenerateItemName()
	{
		return std::format("{} {} {}",
			common::RNG::FromGenerator(itemAdverbs, std::string()),
			common::RNG::FromGenerator(itemAdjectives, std::string()),
			common::RNG::FromGenerator(itemNames, std::string()));
	}

	const std::map<std::string, size_t> peopleNames =
	{
		{"Samuel",1},
		{"Roberta",1},
		{"Davin",1},
		{"Shaniqua",1},
		{"Yermom",1},
		{"David",1},
		{"Sander",1},
		{"Marcus",1},
		{"Barnaby",1},
		{"Ravi",1},
		{"Michael",1},
		{"Guy",1},
		{"Tim",1}
	};

	static std::string GeneratePersonName()
	{
		return common::RNG::FromGenerator(peopleNames, std::string());
	}

	const std::map<std::string, size_t> professionAdjectives =
	{
		{"lousy",1},
		{"smelly",1},
		{"randy",1},
		{"flatulant",1},
		{"grumpy",1},
		{"ugly",1},
		{"miserly",1},
		{"large-breasted",1},
		{"well-endowed",1},
		{"poop eating",1},
		{"stupid",1},
		{"charming",1},
		{"rabid",1}
	};

	const std::map<std::string, size_t> professionNames =
	{
		{"harlot",1},
		{"innkeeper",1},
		{"peasant",1},
		{"leper",1},
		{"merkinsmith",1},
		{"plague doctor",1},
		{"neer-do-well",1},
		{"malcontent",1},
		{"freebooter",1},
		{"pirate",1},
		{"scoundrel",1},
		{"gambler",1},
		{"gravedigger",1},
		{"bogan",1},
		{"dog",1},
		{"knacker",1},
		{"mudlark",1}
	};

	static std::string GenerateProfessionName()
	{
		return std::format("{} {}",
			common::RNG::FromGenerator(professionAdjectives, std::string()),
			common::RNG::FromGenerator(professionNames, std::string()));

	}

	static common::XY<double> GenerateDestination(const common::XY<double>& location)
	{
		auto allIslands = data::game::Island::All();
		size_t index = common::RNG::FromRange(0u, allIslands.size() - 1);
		for (auto& island : allIslands)
		{
			if (island.location != location)
			{
				if (index == 0)
				{
					return island.location;
				}
				index--;
			}
		}
		throw "YOU SHOULD NOT GET HERE!";
	}

	void Update(const common::XY<double>& location)
	{
		auto islandModel = game::Islands::Read(location);
		if (islandModel)
		{
			auto quest = data::game::island::Quests::Read(location);
			if (!quest)
			{
				data::game::island::Quests::QuestData data =
				{
					location,
					GenerateDestination(location),
					GenerateReward(),
					GenerateItemName(),
					GeneratePersonName(),
					GenerateProfessionName()
				};
				data::game::island::Quests::Write(data);
			}
		}
	}

	std::optional<game::Quest::QuestModel> Read(const common::XY<double>& location)
	{
		auto quest = data::game::island::Quests::Read(location);
		if (quest)
		{
			return std::optional<game::Quest::QuestModel>({
				quest.value().destination,
				quest.value().reward,
				quest.value().itemName,
				quest.value().personName,
				quest.value().professionName});
		}
		return std::nullopt;
	}
}