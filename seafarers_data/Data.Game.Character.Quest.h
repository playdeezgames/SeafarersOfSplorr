#pragma once
#include <optional>
#include <string>
namespace data::game::character
{
	struct Quest
	{
		int toIslandId;
		double reward;
		std::string itemName;
		std::string personName;
		std::string professionName;
		std::string receiptEmotion;

		static void Write(int,const std::optional<Quest>&);
		static std::optional<Quest> Read(int);
	};
}
