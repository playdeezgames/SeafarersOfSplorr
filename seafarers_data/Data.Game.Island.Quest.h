#pragma once
#include <Common.XY.h>
#include <optional>
#include <string>
namespace data::game::island::Quest
{
	struct QuestData
	{
		common::XY<double> location;
		common::XY<double> destination;
		double reward;
		std::string itemName;
		std::string personName;
		std::string professionName;
	};

	void Write(const QuestData&);
	std::optional<QuestData> Read(const common::XY<double>&);
	void Clear(const common::XY<double>&);
}
