#include "Game.Islands.Quests.h"
#include <string>
#include "Data.Game.Island.h"
#include "Data.Game.Island.Quests.h"
#include "Common.RNG.h"
#include "Game.Islands.h"
#include <format>
#include <map>
namespace game::islands::Quests
{
	static double GenerateReward()
	{
		return 1.0;//TODO: magic number
	}

	const std::map<std::string, size_t> itemAdverbs = 
	{
		{"woefully",1}
	};
	const std::map<std::string, size_t> itemAdjectives =
	{
		{"turgid",1}
	};
	const std::map<std::string, size_t> itemNames =
	{
		{"macguffin",1}
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
		{"Samuel",1}
	};

	static std::string GeneratePersonName()
	{
		return common::RNG::FromGenerator(peopleNames, std::string());
	}

	const std::map<std::string, size_t> professionAdjectives =
	{
		{"lousy",1}
	};
	const std::map<std::string, size_t> professionNames =
	{
		{"poopsmith",1}
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

	std::optional<QuestModel> Read(const common::XY<double>& location)
	{
		auto quest = data::game::island::Quests::Read(location);
		if (quest)
		{
			return std::optional<QuestModel>({
				quest.value().destination,
				quest.value().reward,
				quest.value().itemName,
				quest.value().personName,
				quest.value().professionName});
		}
		return std::nullopt;
	}

}