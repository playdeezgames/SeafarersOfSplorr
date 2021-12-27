#include "Game.Fisheries.h"
#include "Game.Session.Fisheries.h"
namespace game::session
{
	Fisheries::Fisheries(std::function<std::list<int>()> fisherySource)
		: fisherySource(fisherySource)
	{
	}

	bool Fisheries::HasAny() const
	{
		return !fisherySource().empty();
	}

	void Fisheries::Populate(const Difficulty& difficulty) const
	{
		game::Fisheries::Populate(difficulty);
	}

	void Fisheries::Reset() const
	{
		game::Fisheries::Reset();
	}

	void Fisheries::ApplyTurnEffects() const
	{
		game::Fisheries::ApplyTurnEffects();
	}
}