#pragma once
#include <string>
#include "Game.Achievements.h"
namespace game::avatar
{
	enum class Statistic
	{
		HUNGER,
		HEALTH,
		ATTACK,
		DEFEND,
		ATTACK_TIMER,
		DEFEND_TIMER,
		BOWEL,
		NAUSEA,
		DRUNKENNESS,
		KEYS,
		STREAK,
		WALL_BREAKS
	};
}
namespace game::avatar::Statistics
{
	int Read(const ::game::avatar::Statistic&);
	void Write(const ::game::avatar::Statistic&, int);
	bool IsMinimum(const ::game::avatar::Statistic&);
	bool IsMaximum(const ::game::avatar::Statistic&);
	void Decrease(const ::game::avatar::Statistic&, int);
	void Increase(const ::game::avatar::Statistic&, int);
	void Reset();
	void Start();
	void DoAttackTimer();
	void DoDefendTimer();
	int Maximum(const ::game::avatar::Statistic&);
	int Default(const ::game::avatar::Statistic&);
	const std::string& GetCauseOfDeath();
	const std::optional<game::Statistic>& GetKilledByStatistic();
	const std::optional<game::Achievement>& GetDeathByAchievement();
	void SetCauseOfDeath(const std::string&, const std::optional<game::Statistic>&, const std::optional<game::Achievement>&);
}
