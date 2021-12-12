#pragma once
#include <Common.XY.h>
#include <optional>
#include <string>
namespace game::session
{
	struct Island
	{
		Island(int);
		std::optional<bool> IsKnown() const;
		std::optional<std::string> GetDisplayName() const;
		std::optional<common::XY<double>> GetLocation() const;
	private:
		int islandId;
	};
}
