#include "Game.Islands.h"
#include "Game.Islands.Features.h"
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

	void Islands::Reset(const Difficulty& difficulty) const
	{
		game::Islands::Reset(difficulty);
		game::islands::Features::Reset(difficulty);
	}

	void Islands::ApplyTurnEffects() const
	{
		game::Islands::ApplyTurnEffects();
	}
}