#pragma once
#include "Common.XY.h"
#include <string>
#include <optional>
#include <list>
namespace data::game::Island
{
	struct IslandData
	{
		common::XY<double> location;
		std::string name;
		std::optional<int> visits;
	};

	void Write(const IslandData&);
	std::optional<IslandData> Read(const common::XY<double>&);
	std::list<IslandData> All();
}
