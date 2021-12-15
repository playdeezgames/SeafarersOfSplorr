#include "Game.Session.Islands.h"
namespace game::session
{
	Islands::Islands(std::function<std::list<int>()> islandSource)
		: islandSource(islandSource)
	{

	}

	bool Islands::HasAny() const
	{
		return !islandSource().empty();
	}

	std::optional<Island> Islands::TryGetFirst() const
	{
		if (HasAny())
		{
			return Island(islandSource().front());
		}
		return std::nullopt;
	}

	Island Islands::GetFirst() const
	{
		return TryGetFirst().value();
	}

	size_t Islands::GetCount() const
	{
		return islandSource().size();
	}

	std::list<Island> Islands::GetAll() const
	{
		std::list<Island> result;
		for (auto islandId : islandSource())
		{
			result.push_back(Island(islandId));
		}
		return result;
	}
}