#include "Game.Avatar.Statistics.h"
#include <map>
#include "json.hpp"
#include "Game.Data.Properties.h"
#include "Game.Avatar.h"
#include "Data.JSON.h"
#include "Common.Data.Properties.h"
#include "Common.Utility.h"
#include "Data.Stores.h"
#include <format>
#include "Game.Achievements.h"
namespace game::avatar::Statistics
{
	static const nlohmann::json& GetStatistic(game::avatar::Statistic statistic)
	{
		return ::data::Stores::GetStore(::data::Store::STATISTICS)[std::format("{}", (int)statistic)];
	}

	nlohmann::json& GetAvatarStatistics()
	{
		auto& avatar = game::Avatar::GetAvatar();
		if (avatar.count(game::data::Properties::STATISTICS) == 0)
		{
			avatar[game::data::Properties::STATISTICS] = nlohmann::json();
		}
		return avatar[game::data::Properties::STATISTICS];
	}

	int Read(const ::game::avatar::Statistic& statistic)
	{
		return GetAvatarStatistics()[(int)statistic];
	}

	int Minimum(const ::game::avatar::Statistic& statistic)
	{
		return (int)GetStatistic(statistic)[common::data::Properties::MINIMUM];
	}

	int Maximum(const ::game::avatar::Statistic& statistic)
	{
		return (int)GetStatistic(statistic)[common::data::Properties::MAXIMUM];
	}

	int Default(const ::game::avatar::Statistic& statistic)
	{
		return (int)GetStatistic(statistic)[common::data::Properties::DEFAULT];
	}

	void Write(const ::game::avatar::Statistic& statistic, int value)
	{
		const auto minimum = Minimum(statistic);
		const auto maximum = Maximum(statistic);
		GetAvatarStatistics()[(int)statistic] =
			(value < minimum) ? (minimum) :
			(value > maximum) ? (maximum) :
			(value);
	}

	bool IsMinimum(const ::game::avatar::Statistic& statistic)
	{
		return Read(statistic) == Minimum(statistic);
	}

	bool IsMaximum(const ::game::avatar::Statistic& statistic)
	{
		return Read(statistic) == Maximum(statistic);
	}

	void Reset()
	{
		for (auto& item : ::data::Stores::GetStore(::data::Store::STATISTICS).items())
		{
			game::avatar::Statistic statistic = (game::avatar::Statistic)common::Utility::StringToInt(item.key());
			Write(statistic, Default(statistic));
		}
	}

	void Start()
	{
		Reset();
	}

	void Decrease(const ::game::avatar::Statistic& statistic, int delta)
	{
		Write(statistic, Read(statistic) - delta);
	}

	void Increase(const ::game::avatar::Statistic& statistic, int delta)
	{
		Decrease(statistic, -delta);
	}

	void DoAttackTimer()
	{
		auto timer = game::avatar::Statistics::Read(game::avatar::Statistic::ATTACK_TIMER);
		if (timer > 0)
		{
			timer--;
			game::avatar::Statistics::Write(game::avatar::Statistic::ATTACK_TIMER, timer);
			if (timer == 0)
			{
				game::avatar::Statistics::Write(game::avatar::Statistic::ATTACK, game::avatar::Statistics::Default(game::avatar::Statistic::ATTACK));
			}
		}
	}

	void DoDefendTimer()
	{
		auto timer = game::avatar::Statistics::Read(game::avatar::Statistic::DEFEND_TIMER);
		if (timer > 0)
		{
			timer--;
			game::avatar::Statistics::Write(game::avatar::Statistic::DEFEND_TIMER, timer);
			if (timer == 0)
			{
				game::avatar::Statistics::Write(game::avatar::Statistic::DEFEND, game::avatar::Statistics::Default(game::avatar::Statistic::DEFEND));
			}
		}
	}

	static std::string causeOfDeath;
	static std::optional<game::Statistic> killedByStatistic;
	static std::optional<game::Achievement> deathByAchievement;

	const std::string& GetCauseOfDeath()
	{
		return causeOfDeath;
	}

	const std::optional<game::Statistic>& GetKilledByStatistic()
	{
		return killedByStatistic;
	}

	const std::optional<game::Achievement>& GetDeathByAchievement()
	{
		return deathByAchievement;
	}

	void SetCauseOfDeath(const std::string& cod, const std::optional<game::Statistic>& statistic, const std::optional<game::Achievement>& achievement)
	{
		causeOfDeath = cod;
		killedByStatistic = statistic;
		deathByAchievement = achievement;
	}
}