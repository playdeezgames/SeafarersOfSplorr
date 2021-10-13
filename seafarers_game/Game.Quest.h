#pragma once
#include <Common.XY.h>
#include <string>
namespace game//20211013
{
	struct Quest
	{
		common::XY<double> destination;
		double reward;
		std::string itemName;
		std::string personName;
		std::string professionName;
		std::string receiptEmotion;
	};
}
