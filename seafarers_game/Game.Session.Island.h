#pragma once
#include <optional>
#include <string>
namespace game::session
{
	struct Island
	{
		Island(int);
		std::optional<bool> IsKnown() const;
		std::optional<std::string> GetDisplayName() const;
	private:
		int islandId;
	};
}
