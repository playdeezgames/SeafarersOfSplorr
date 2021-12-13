#pragma once
#include <Common.XY.h>
#include <optional>
#include <string>
namespace game::session
{
	struct Island
	{
		Island(int);
		bool IsKnown() const;
		std::string GetDisplayName() const;
		common::XY<double> GetLocation() const;
	private:
		int islandId;
	};
}
