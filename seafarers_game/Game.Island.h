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
		bool isKnown;
		int patronDemigodId;
		std::string GetDisplayName()
		{
			if (isKnown)
			{
				return name;
			}
			else
			{
				return "(unknown)";
			}
		}
	};
}