#pragma once
#include <Common.XY.h>
#include <optional>
#include <string>
namespace game
{
	struct Island
	{
		int id;
		common::XY<double> relativeLocation;
		common::XY<double> absoluteLocation;
		std::string name;
		std::optional<int> visits;
		int patronDemigodId;
		std::string GetDisplayName()
		{
			if (visits.has_value())
			{
				return name;
			}
			else
			{
				return "(unknown)";
			}
		}
		bool IsKnown()
		{
			return visits.has_value();
		}
	};
}