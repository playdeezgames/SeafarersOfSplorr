#pragma once
#include "Common.XY.h"
#include <string>
#include <optional>
namespace data::game::avatar::Quest
{
	struct QuestData
	{
		common::XY<double> destination;
		double reward;
		std::string itemName;
		std::string personName;
		std::string professionName;
	};

	void Write(const std::optional<QuestData>&);
	std::optional<QuestData> Read();
}
