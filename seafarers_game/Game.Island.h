#pragma once
#include <Common.XY.h>
#include <optional>
#include <string>
namespace game//20211017
{
	struct Island
	{
		common::XY<double> relativeLocation;
		common::XY<double> absoluteLocation;
		std::string name;
		std::optional<int> visits;
		std::string patronDemigod;
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