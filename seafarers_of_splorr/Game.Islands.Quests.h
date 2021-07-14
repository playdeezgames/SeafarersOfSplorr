#pragma once
#include "Common.XY.h"
#include <string>
#include <optional>
namespace game::islands::Quests
{
	struct QuestModel
	{
		common::XY<double> destination;
		double reward;
		std::string itemName;
		std::string personName;
		std::string professionName;
	};
	void Update(const common::XY<double>&);
	std::optional<QuestModel> Read(const common::XY<double>&);
}
