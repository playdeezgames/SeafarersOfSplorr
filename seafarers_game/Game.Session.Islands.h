#pragma once
#include "Game.Session.Island.h"
#include <functional>
#include <optional>
#include <list>
namespace game::session
{
	struct Islands
	{
		Islands(std::function<std::list<int>()>);
		bool HasAny() const;
		std::optional<Island> TryGetFirst() const;
		Island GetFirst() const;
		size_t GetCount() const;
		std::list<Island> GetAll() const;
	private:
		std::function<std::list<int>()> islandSource;
	};
}