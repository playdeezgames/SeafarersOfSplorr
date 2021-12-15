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
}