#pragma once
#include <optional>
namespace game::session::character
{
	struct HitPoints
	{
		HitPoints(int);
		int GetMaximum() const;
		int GetCurrent() const;
		void Change(int) const;
	private:
		int characterId;
	};
}
