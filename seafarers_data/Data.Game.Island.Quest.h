#pragma once
#include <optional>
#include <string>
namespace data::game::island
{
	struct Quest
	{
		int fromIslandId;
		int toIslandId;
		double reward;
		std::string itemName;
		std::string personName;
		std::string professionName;
		std::string receiptEmotion;

		static void Write(const Quest&);
		static std::optional<Quest> Read(int);
		static void Clear(int);
	};

}
