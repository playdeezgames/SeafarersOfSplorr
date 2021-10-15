#pragma once
#include <optional>
namespace game//20211013
{
	enum class Achievement
	{
		YER_DEAD
	};
	enum class Statistic
	{
		GAMES_PLAYED,
		MOVES_MADE
	};
}
namespace game::Statistics//20211013
{
	void Increment(const game::Statistic&);
	std::optional<int> Read(const game::Statistic&);
}
namespace game::Achievements//20211013
{
	void Add(const game::Achievement&);
	void Start();
	bool ShouldRestartApp();
}
