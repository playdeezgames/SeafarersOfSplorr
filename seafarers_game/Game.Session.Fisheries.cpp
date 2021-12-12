#include "Game.Session.Fisheries.h"
namespace game::session
{
	Fisheries::Fisheries(const std::list<int>& fisheryIds)
		: fisheryIds(fisheryIds)
	{

	}

	bool Fisheries::HasAny() const
	{
		return !fisheryIds.empty();
	}

}