#pragma once
#include <functional>
#include "Game.Difficulty.h"
#include "Game.Session.Island.h"
#include <optional>
#include <list>
namespace game::session
{
	struct IslandDeprecated
	{
		IslandDeprecated(std::function<std::list<int>()>);
		bool HasAny() const;
		std::optional<Island> TryGetFirst() const;
		Island GetFirst() const;
		size_t GetCount() const;
		std::list<Island> GetAll() const;
		void Reset(const Difficulty&) const;
		void ApplyTurnEffects() const;
		Island GetIsland(int) const;
	private:
		std::function<std::list<int>()> islandSource;
	};
}