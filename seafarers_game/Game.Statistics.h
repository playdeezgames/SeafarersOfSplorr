#pragma once
#include <optional>
#include "Game.Statistic.h"
namespace game::Statistics
{
	void Increment(const game::Statistic&);
	std::optional<int> Read(const game::Statistic&);
}