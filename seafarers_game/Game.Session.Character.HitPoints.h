#pragma once
#include <optional>
namespace game::session::character
{
	struct HitPoints
	{
		HitPoints(int);
		std::optional<int> GetMaximum() const;
		std::optional<int> GetCurrent() const;
	private:
		int characterId;
	};
}
