#include "Game.Islands.h"
#include "Game.Islands.Features.h"
#include "Game.Session.IslandsDeprecated.h"
namespace game::session
{
	IslandDeprecated::IslandDeprecated(std::function<std::list<int>()> islandSource)
		: islandSource(islandSource)
	{

	}

	bool IslandDeprecated::HasAny() const
	{
		return !islandSource().empty();
	}

	std::optional<Island> IslandDeprecated::TryGetFirst() const
	{
		if (HasAny())
		{
			return Island(islandSource().front());
		}
		return std::nullopt;
	}

	Island IslandDeprecated::GetFirst() const
	{
		return TryGetFirst().value();
	}

	size_t IslandDeprecated::GetCount() const
	{
		return islandSource().size();
	}

	std::list<Island> IslandDeprecated::GetAll() const
	{
		std::list<Island> result;
		for (auto islandId : islandSource())
		{
			result.push_back(Island(islandId));
		}
		return result;
	}

	void IslandDeprecated::Reset(const Difficulty& difficulty) const
	{
		game::Islands::Reset(difficulty);
		game::islands::Features::Reset(difficulty);
	}

	void IslandDeprecated::ApplyTurnEffects() const
	{
		game::Islands::ApplyTurnEffects();
	}

	Island IslandDeprecated::GetIsland(int islandId) const
	{
		return Island(islandId);
	}
}