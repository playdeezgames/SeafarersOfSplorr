#pragma once
#include "Game.Difficulty.h"
#include <functional>
#include <list>
namespace game::session
{
	struct Fisheries
	{
		Fisheries(std::function<std::list<int>()>);
		bool HasAny() const;
		void Reset(const Difficulty&) const;
		void Populate(const Difficulty&) const;
		void ApplyTurnEffects() const;
	private:
		std::function<std::list<int>()> fisherySource;
	};
}
