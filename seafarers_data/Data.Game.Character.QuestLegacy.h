#pragma once
#include <optional>
#include <string>
namespace data::game::character
{
	struct QuestLegacy
	{
		int toIslandId;
		double reward;
		std::string itemName;
		std::string personName;
		std::string professionName;
		std::string receiptEmotion;

		static void Write(int,const std::optional<QuestLegacy>&);
		static std::optional<QuestLegacy> Read(int);
	};
}
