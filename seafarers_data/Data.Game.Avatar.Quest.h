#pragma once
#include <Common.XY.h>
#include <optional>
#include <string>
namespace data::game::avatar
{
	struct Quest
	{
		common::XY<double> destination;
		double reward;
		std::string itemName;
		std::string personName;
		std::string professionName;
		std::string receiptEmotion;

		static void Write(int,const std::optional<Quest>&);
		static std::optional<Quest> Read(int);
	};
}
