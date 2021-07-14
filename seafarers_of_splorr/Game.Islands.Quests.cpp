#include "Game.Islands.Quests.h"
#include <string>
#include "Data.Game.Island.h"
#include "Data.Game.Island.Quests.h"
#include "Common.RNG.h"
#include "Game.Islands.h"
namespace game::islands::Quests
{
	static double GenerateReward()
	{
		return 1.0;//TODO: magic number
	}

	static std::string GenerateItemName()
	{
		return "macguffin";//TODO: magic string
	}

	static std::string GeneratePersonName()
	{
		return "Samuel";//TODO: magic string
	}

	static std::string GenerateProfessionName()
	{
		return "Poopsmith";//TODO: magic string
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

}