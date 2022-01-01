#pragma once
#include <optional>
namespace game::session::character
{
	struct HitPoints
	{
		constexpr explicit HitPoints(int characterId) : characterId(characterId) { }
		int GetMaximum() const;
		int GetCurrent() const;
		void Change(int delta) const;
	private:
		int characterId;
	};
}
