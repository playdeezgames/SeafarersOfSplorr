#pragma once
#include "Game.Difficulty.h"
#include "Game.Session.World.Calendar.Months.h"
#include <string>
namespace game::session::world
{
	struct Calendar
	{
		constexpr Calendar(int worldId) : worldId(worldId) {}
		constexpr calendar::Months GetMonths() const { return calendar::Months(worldId); }
		std::string GetDate() const;
		int GetYearLength() const;
		void Populate(const Difficulty&) const;
		void Reset() const;
		void ApplyTurnEffects() const;
	private:
		int worldId;
	};
}
