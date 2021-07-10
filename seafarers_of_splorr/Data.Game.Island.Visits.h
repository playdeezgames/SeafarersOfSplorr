#pragma once
#include <optional>
#include "Common.XY.h"
namespace data::game::island::Visits
{
	struct VisitData
	{
		common::XY<double> location;
		int visits;
		std::optional<int> lastVisit;
	};

	void Write(const VisitData&);
	std::optional<VisitData> Read(const common::XY<double>&);
	void Clear();
}