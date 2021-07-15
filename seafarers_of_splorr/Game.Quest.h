#pragma once
#include "Common.XY.h"
#include <string>
namespace game::Quest
{
	struct QuestModel
	{
		common::XY<double> destination;
		double reward;
		std::string itemName;
		std::string personName;
		std::string professionName;
	};
}
