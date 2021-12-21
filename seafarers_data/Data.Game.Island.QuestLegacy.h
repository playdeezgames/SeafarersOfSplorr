#pragma once
#include <optional>
#include <string>
namespace data::game::island
{
	struct QuestLegacy
	{
		int fromIslandId;
		int toIslandId;
		double reward;
		std::string itemName;
		std::string personName;
		std::string professionName;
		std::string receiptEmotion;

		static void Write(const QuestLegacy&);
		static std::optional<QuestLegacy> Read(int);
		static void Clear(int);
	};

}
