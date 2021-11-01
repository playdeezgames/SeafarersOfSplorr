#include <Common.Utility.h>
#include <Common.Utility.Optional.h>
#include <Common.Utility.Table.h>
#include <Data.Game.Avatar.Statistic.h>
#include <Data.Game.Common.h>
#include "Game.Avatar.Equipment.h"
#include "Game.Avatar.Statistic.h"
#include "Game.Avatar.Statistics.h"
#include <Game.Item.h>
#include <Game.Player.h>
#include <map>
#include <list>
namespace game::avatar
{
	struct StatisticDescriptor
	{
		game::avatar::Statistic statistic;
		std::optional<double> minimum;
		std::optional<double> maximum;
		double initial;
	};

	static const std::map<Item, double> dignityBuffs =
	{
		{ Item::TROUSERS, 100.0 },
		{ Item::POSH_TROUSERS, 100.0 }
	};

	static const std::map<Item, double> poshBuffs =
	{
		{ Item::POSH_TROUSERS, 100.0 }
	};

	static const std::map<game::avatar::Statistic, std::map<Item, double>> allBuffs =
	{
		{Statistic::DIGNITY, dignityBuffs},
		{Statistic::POSHNESS, poshBuffs}
	};

	static const std::map<game::Difficulty, std::list<StatisticDescriptor>> initialValues =
	{
		{game::Difficulty::EASY,
			{
				{game::avatar::Statistic::TURNS_REMAINING, std::optional<double>(0.0), std::optional<double>(10000.0), 10000.0},
				{game::avatar::Statistic::HEALTH, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::avatar::Statistic::SATIETY, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::avatar::Statistic::REPUTATION, std::nullopt, std::nullopt, 0.0},
				{game::avatar::Statistic::MONEY, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::avatar::Statistic::BRAWLING, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::avatar::Statistic::INFAMY, std::optional<double>(0.0), std::nullopt, 0.0},
				{
					game::avatar::Statistic::DIGNITY, 
					std::optional<double>(0.0), 
					std::optional<double>(100.0), 
					0.0
				},
				{game::avatar::Statistic::POSHNESS, std::optional<double>(0.0), std::nullopt, 0.0}
			}},
		{game::Difficulty::NORMAL,
			{
				{game::avatar::Statistic::TURNS_REMAINING, std::optional<double>(0.0), std::optional<double>(10000.0), 10000.0},
				{game::avatar::Statistic::HEALTH, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::avatar::Statistic::SATIETY, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::avatar::Statistic::REPUTATION, std::nullopt, std::nullopt, 0.0},
				{game::avatar::Statistic::MONEY, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::avatar::Statistic::BRAWLING, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::avatar::Statistic::INFAMY, std::optional<double>(0.0), std::nullopt, 0.0},
				{
					game::avatar::Statistic::DIGNITY,
					std::optional<double>(0.0),
					std::optional<double>(100.0),
					0.0
				},
				{game::avatar::Statistic::POSHNESS, std::optional<double>(0.0), std::nullopt, 0.0}
			}},
		{game::Difficulty::HARD,
			{
				{game::avatar::Statistic::TURNS_REMAINING, std::optional<double>(0.0), std::optional<double>(10000.0), 10000.0},
				{game::avatar::Statistic::HEALTH, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::avatar::Statistic::SATIETY, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::avatar::Statistic::REPUTATION, std::nullopt, std::nullopt, 0.0},
				{game::avatar::Statistic::MONEY, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::avatar::Statistic::BRAWLING, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::avatar::Statistic::INFAMY, std::optional<double>(0.0), std::nullopt, 0.0},
				{
					game::avatar::Statistic::DIGNITY,
					std::optional<double>(0.0),
					std::optional<double>(100.0),
					0.0
				},
				{game::avatar::Statistic::POSHNESS, std::optional<double>(0.0), std::nullopt, 0.0}
			}},
		{game::Difficulty::HARDCORE,
			{
				{game::avatar::Statistic::TURNS_REMAINING, std::optional<double>(0.0), std::optional<double>(10000.0), 10000.0},
				{game::avatar::Statistic::HEALTH, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::avatar::Statistic::SATIETY, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::avatar::Statistic::REPUTATION, std::nullopt, std::nullopt, 0.0},
				{game::avatar::Statistic::MONEY, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::avatar::Statistic::BRAWLING, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::avatar::Statistic::INFAMY, std::optional<double>(0.0), std::nullopt, 0.0},
				{
					game::avatar::Statistic::DIGNITY,
					std::optional<double>(0.0),
					std::optional<double>(100.0),
					0.0
				},
				{game::avatar::Statistic::POSHNESS, std::optional<double>(0.0), std::nullopt, 0.0}
			}}
	};

	void Statistics::Reset(const game::Difficulty& difficulty)
	{
		auto values = initialValues.find(difficulty)->second;
		for (auto& value : values)
		{
			data::game::avatar::Statistic::Write(
				Player::GetAvatarId(),
				(int)value.statistic,
				{
					value.minimum,
					value.maximum,
					value.initial
				});
		}
	}

	static std::optional<double> GetMaximum(int avatarId, const game::avatar::Statistic& statistic)
	{
		return data::game::avatar::Statistic::Read(avatarId, (int)statistic).value().maximum;
	}

	static std::optional<double> GetMinimum(int avatarId, const game::avatar::Statistic& statistic)
	{
		return data::game::avatar::Statistic::Read(avatarId, (int)statistic).value().minimum;
	}

	static double GetCurrent(int avatarId, const game::avatar::Statistic& statistic)
	{
		return data::game::avatar::Statistic::Read(avatarId, (int)statistic).value().current;
	}

	static double CalculateBuffs(const std::map<game::Item, double> itemBuffs)
	{
		return common::utility::Table::Accumulate<game::Item, double, double>(
			itemBuffs,
			[](const double& result, const game::Item& item, const double& buff)
			{
				return result + (game::avatar::Equipment::IsEquipped(item)) ? (buff) : (0.0);
			});
	}

	static double GetCurrentWithBuffs(int avatarId, const game::avatar::Statistic& statistic)
	{
		return 
			GetCurrent(avatarId, statistic) +
			common::utility::Optional::Map<std::map<game::Item, double>, double>(
				common::utility::Table::TryGetKey(allBuffs, statistic),
				CalculateBuffs).value_or(0.0);
	}

	static void SetCurrent(int avatarId, const game::avatar::Statistic& statistic, double value)
	{
		auto data = data::game::avatar::Statistic::Read(avatarId, (int)statistic).value();
		data.current = common::Utility::Clamp<double>(value, data.minimum, data.maximum);
		data::game::avatar::Statistic::Write(
			avatarId,
			(int)statistic, 
			data);
	}

	static double ChangeCurrent(int avatarId, const game::avatar::Statistic& statistic, double delta)
	{
		SetCurrent(avatarId, statistic, GetCurrent(avatarId, statistic) + delta);
		return GetCurrent(avatarId, statistic);
	}

	double Statistics::ReadMoney(int avatarId)
	{
		return GetCurrent(avatarId, game::avatar::Statistic::MONEY);
	}

	double Statistics::GetHealth(int avatarId)
	{
		return GetCurrent(avatarId, game::avatar::Statistic::HEALTH);
	}

	double Statistics::GetSatiety(int avatarId)
	{
		return GetCurrent(avatarId, game::avatar::Statistic::SATIETY);
	}

	double Statistics::GetInfamy(int avatarId)
	{
		return GetCurrent(avatarId, game::avatar::Statistic::INFAMY);
	}

	double Statistics::GetBrawling(int avatarId)
	{
		return GetCurrent(avatarId, game::avatar::Statistic::BRAWLING);
	}

	static double GetDownAmount(int avatarId, const Statistic& statistic)
	{
		return GetMaximum(avatarId, statistic).value() - GetCurrent(avatarId, statistic);
	}

	void Statistics::Eat(int avatarId, double amount)
	{
		double healthDown = GetDownAmount(avatarId, Statistic::HEALTH);
		if (healthDown > 0)
		{
			ChangeCurrent(avatarId, Statistic::HEALTH, amount);
			amount -= healthDown;
		}
		if (amount > 0)
		{
			ChangeCurrent(avatarId, Statistic::SATIETY, amount);
		}
	}

	bool Statistics::NeedToEat(int avatarId, double amount)
	{
		double totalDown =
			GetDownAmount(avatarId, game::avatar::Statistic::HEALTH)+
			GetDownAmount(avatarId, game::avatar::Statistic::SATIETY);
		return totalDown >= amount;
	}

	double Statistics::GetReputation(int avatarId)
	{
		return GetCurrent(avatarId, game::avatar::Statistic::REPUTATION);
	}

	void Statistics::ChangeReputation(int avatarId, double delta)
	{
		ChangeCurrent(avatarId, game::avatar::Statistic::REPUTATION, delta);
	}

	int Statistics::GetTurnsRemaining(int avatarId)
	{
		return (int)GetCurrent(avatarId, game::avatar::Statistic::TURNS_REMAINING);
	}

	void Statistics::SpendTurn(int avatarId)
	{
		const double TURN_DELTA = -1.0;
		ChangeCurrent(avatarId, game::avatar::Statistic::TURNS_REMAINING, TURN_DELTA);
	}

	bool Statistics::IsOutOfTurns(int avatarId)
	{
		return GetCurrent(avatarId, game::avatar::Statistic::TURNS_REMAINING) <= GetMinimum(avatarId, game::avatar::Statistic::TURNS_REMAINING);
	}

	static bool IsMinimal(int avatarId, const Statistic& statistic)
	{
		return GetCurrent(avatarId, statistic) <= GetMinimum(avatarId, statistic);
	}

	bool Statistics::IsDead(int avatarId)
	{
		return IsMinimal(avatarId, Statistic::HEALTH);
	}

	bool Statistics::IsStarving(int avatarId)
	{
		return IsMinimal(avatarId, Statistic::SATIETY);
	}

	void Statistics::ChangeMoney(int avatarId, double delta)
	{
		ChangeCurrent(avatarId, Statistic::MONEY, delta);
	}

	void Statistics::ChangeHealth(int avatarId, double delta)
	{
		ChangeCurrent(avatarId, Statistic::HEALTH, delta);
	}

	void Statistics::ChangeSatiety(int avatarId, double delta)
	{
		ChangeCurrent(avatarId, Statistic::SATIETY, delta);
	}

	void Statistics::ChangeInfamy(int avatarId, double delta)
	{
		ChangeCurrent(avatarId, Statistic::INFAMY, delta);
	}

	void Statistics::ChangeBrawling(int avatarId, double delta)
	{
		ChangeCurrent(avatarId, Statistic::BRAWLING, delta);
	}

	double Statistics::GetDignity(int avatarId)
	{
		return GetCurrentWithBuffs(avatarId, avatar::Statistic::DIGNITY);
	}

	double Statistics::GetPoshness(int avatarId)
	{
		return GetCurrentWithBuffs(avatarId, avatar::Statistic::POSHNESS);
	}

	bool Statistics::IsPlayerDead()
	{
		return IsDead(Player::GetAvatarId());
	}

	bool Statistics::IsPlayerOutOfTurns()
	{
		return IsOutOfTurns(Player::GetAvatarId());
	}
}