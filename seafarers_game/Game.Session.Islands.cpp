#include "Game.Session.Islands.h"
namespace game::session
{
	Islands::Islands(const std::list<int>& islandIds)
		: islandIds(islandIds)
	{

	}

	bool Islands::HasAny() const
	{
		return !islandIds.empty();
	}

	std::optional<Island> Islands::TryGetFirst() const
	{
		if (HasAny())
		{
			return Island(islandIds.front());
		}
		return std::nullopt;
	}

	Island Islands::GetFirst() const
	{
		return TryGetFirst().value();
	}

	size_t Islands::GetCount() const
	{
		return islandIds.size();
	}

	std::list<Island> Islands::GetAll() const
	{
		std::list<Island> result;
		for (auto islandId : islandIds)
		{
			result.push_back(Island(islandId));
		}
		return result;
	}
}