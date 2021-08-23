#pragma once
#include <Common.XY.h>
#include <optional>
#include <string>
namespace data::game::avatar
{
	class Quest
	{
	public:
		struct Data
		{
			common::XY<double> destination;
			double reward;
			std::string itemName;
			std::string personName;
			std::string professionName;
		};
		static void Write(const std::optional<Data>&);
		static std::optional<Data> Read();
	};
}
