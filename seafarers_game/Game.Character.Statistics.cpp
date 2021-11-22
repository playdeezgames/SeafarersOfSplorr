#include <Common.Utility.h>
#include <Common.Utility.Optional.h>
#include <Common.Utility.Table.h>
#include <Data.Game.Character.Statistic.h>
#include <Data.Game.Common.h>
#include "Game.Character.Equipment.h"
#include "Game.Character.Statistic.h"
#include "Game.Character.Statistics.h"
#include "Game.Item.h"
#include "Game.Player.h"
#include <map>
#include <list>
namespace game::character
{
	struct StatisticDescriptor
	{
		game::character::Statistic statistic;
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

	static const std::map<game::character::Statistic, std::map<Item, double>> allBuffs =
	{
		{Statistic::DIGNITY, dignityBuffs},
		{Statistic::POSHNESS, poshBuffs}
	};

	static const std::map<game::Difficulty, std::list<StatisticDescriptor>> initialValues =
	{
		{game::Difficulty::EASY,
			{
				{game::character::Statistic::TURNS_REMAINING, std::optional<double>(0.0), std::optional<double>(10000.0), 10000.0},
				{game::character::Statistic::HEALTH, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::character::Statistic::SATIETY, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::character::Statistic::REPUTATION, std::nullopt, std::nullopt, 0.0},
				{game::character::Statistic::MONEY, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::character::Statistic::BRAWLING, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::character::Statistic::INFAMY, std::optional<double>(0.0), std::nullopt, 0.0},
				{
					game::character::Statistic::DIGNITY,
					std::optional<double>(0.0), 
					std::optional<double>(100.0), 
					0.0
				},
				{game::character::Statistic::POSHNESS, std::optional<double>(0.0), std::nullopt, 0.0}
			}},
		{game::Difficulty::NORMAL,
			{
				{game::character::Statistic::TURNS_REMAINING, std::optional<double>(0.0), std::optional<double>(10000.0), 10000.0},
				{game::character::Statistic::HEALTH, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::character::Statistic::SATIETY, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::character::Statistic::REPUTATION, std::nullopt, std::nullopt, 0.0},
				{game::character::Statistic::MONEY, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::character::Statistic::BRAWLING, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::character::Statistic::INFAMY, std::optional<double>(0.0), std::nullopt, 0.0},
				{
					game::character::Statistic::DIGNITY,
					std::optional<double>(0.0),
					std::optional<double>(100.0),
					0.0
				},
				{game::character::Statistic::POSHNESS, std::optional<double>(0.0), std::nullopt, 0.0}
			}},
		{game::Difficulty::HARD,
			{
				{game::character::Statistic::TURNS_REMAINING, std::optional<double>(0.0), std::optional<double>(10000.0), 10000.0},
				{game::character::Statistic::HEALTH, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::character::Statistic::SATIETY, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::character::Statistic::REPUTATION, std::nullopt, std::nullopt, 0.0},
				{game::character::Statistic::MONEY, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::character::Statistic::BRAWLING, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::character::Statistic::INFAMY, std::optional<double>(0.0), std::nullopt, 0.0},
				{
					game::character::Statistic::DIGNITY,
					std::optional<double>(0.0),
					std::optional<double>(100.0),
					0.0
				},
				{game::character::Statistic::POSHNESS, std::optional<double>(0.0), std::nullopt, 0.0}
			}},
		{game::Difficulty::HARDCORE,
			{
				{game::character::Statistic::TURNS_REMAINING, std::optional<double>(0.0), std::optional<double>(10000.0), 10000.0},
				{game::character::Statistic::HEALTH, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::character::Statistic::SATIETY, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::character::Statistic::REPUTATION, std::nullopt, std::nullopt, 0.0},
				{game::character::Statistic::MONEY, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::character::Statistic::BRAWLING, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::character::Statistic::INFAMY, std::optional<double>(0.0), std::nullopt, 0.0},
				{
					game::character::Statistic::DIGNITY,
					std::optional<double>(0.0),
					std::optional<double>(100.0),
					0.0
				},
				{game::character::Statistic::POSHNESS, std::optional<double>(0.0), std::nullopt, 0.0}
			}}
	};

	void Statistics::Reset(const game::Difficulty& difficulty)
	{
		auto values = initialValues.find(difficulty)->second;
		for (auto& value : values)
		{
			data::game::character::Statistic::Write(
				Player::GetAvatarId(),
				(int)value.statistic,
				{
					value.minimum,
					value.maximum,
					value.initial
				});
		}
	}

	static std::optional<double> GetMaximum(int characterId, const game::character::Statistic& statistic)
	{
		return data::game::character::Statistic::Read(characterId, (int)statistic).value().maximum;
	}

	static std::optional<double> GetMinimum(int characterId, const game::character::Statistic& statistic)
	{
		return data::game::character::Statistic::Read(characterId, (int)statistic).value().minimum;
	}

	static double GetCurrent(int characterId, const game::character::Statistic& statistic)
	{
		return data::game::character::Statistic::Read(characterId, (int)statistic).value().current;
	}

	static double CalculateBuffs(int characterId, const std::map<game::Item, double> itemBuffs)
	{
		return common::utility::Table::Accumulate<game::Item, double, double>(
			itemBuffs,
			[characterId](const double& result, const game::Item& item, const double& buff)
			{
				return result + (game::character::Equipment::IsEquipped(characterId, item)) ? (buff) : (0.0);
			});
	}

	static double GetCurrentWithBuffs(int characterId, const game::character::Statistic& statistic)
	{
		auto result = GetCurrent(characterId, statistic);
		auto blah = common::utility::Table::TryGetKey(allBuffs, statistic);
		if (blah)
		{
			result += CalculateBuffs(characterId, blah.value());
		}
		return result;
	}

	static void SetCurrent(int characterId, const game::character::Statistic& statistic, double value)
	{
		auto data = data::game::character::Statistic::Read(characterId, (int)statistic).value();
		data.current = common::Utility::Clamp<double>(value, data.minimum, data.maximum);
		data::game::character::Statistic::Write(
			characterId,
			(int)statistic, 
			data);
	}

	static double ChangeCurrent(int characterId, const game::character::Statistic& statistic, double delta)
	{
		SetCurrent(characterId, statistic, GetCurrent(characterId, statistic) + delta);
		return GetCurrent(characterId, statistic);
	}

	double Statistics::ReadMoney(int characterId)
	{
		return GetCurrent(characterId, game::character::Statistic::MONEY);
	}

	double Statistics::GetHealth(int characterId)
	{
		return GetCurrent(characterId, game::character::Statistic::HEALTH);
	}

	double Statistics::GetSatiety(int characterId)
	{
		return GetCurrent(characterId, game::character::Statistic::SATIETY);
	}

	double Statistics::GetInfamy(int characterId)
	{
		return GetCurrent(characterId, game::character::Statistic::INFAMY);
	}

	double Statistics::GetBrawling(int characterId)
	{
		return GetCurrent(characterId, game::character::Statistic::BRAWLING);
	}

	static double GetDownAmount(int characterId, const Statistic& statistic)
	{
		return GetMaximum(characterId, statistic).value() - GetCurrent(characterId, statistic);
	}

	void Statistics::Eat(int characterId, double amount)
	{
		double healthDown = GetDownAmount(characterId, Statistic::HEALTH);
		if (healthDown > 0)
		{
			ChangeCurrent(characterId, Statistic::HEALTH, amount);
			amount -= healthDown;
		}
		if (amount > 0)
		{
			ChangeCurrent(characterId, Statistic::SATIETY, amount);
		}
	}

	bool Statistics::NeedToEat(int characterId, double amount)
	{
		double totalDown =
			GetDownAmount(characterId, game::character::Statistic::HEALTH)+
			GetDownAmount(characterId, game::character::Statistic::SATIETY);
		return totalDown >= amount;
	}

	double Statistics::GetReputation(int characterId)
	{
		return GetCurrent(characterId, game::character::Statistic::REPUTATION);
	}

	void Statistics::ChangeReputation(int characterId, double delta)
	{
		ChangeCurrent(characterId, game::character::Statistic::REPUTATION, delta);
	}

	int Statistics::GetTurnsRemaining(int characterId)
	{
		return (int)GetCurrent(characterId, game::character::Statistic::TURNS_REMAINING);
	}

	void Statistics::SpendTurn(int characterId)
	{
		const double TURN_DELTA = -1.0;
		ChangeCurrent(characterId, game::character::Statistic::TURNS_REMAINING, TURN_DELTA);
	}

	bool Statistics::IsOutOfTurns(int characterId)
	{
		return GetCurrent(characterId, game::character::Statistic::TURNS_REMAINING) <= GetMinimum(characterId, game::character::Statistic::TURNS_REMAINING);
	}

	static bool IsMinimal(int characterId, const Statistic& statistic)
	{
		return GetCurrent(characterId, statistic) <= GetMinimum(characterId, statistic);
	}

	bool Statistics::IsDead(int characterId)
	{
		return IsMinimal(characterId, Statistic::HEALTH);
	}

	bool Statistics::IsStarving(int characterId)
	{
		return IsMinimal(characterId, Statistic::SATIETY);
	}

	void Statistics::ChangeMoney(int characterId, double delta)
	{
		ChangeCurrent(characterId, Statistic::MONEY, delta);
	}

	void Statistics::ChangeHealth(int characterId, double delta)
	{
		ChangeCurrent(characterId, Statistic::HEALTH, delta);
	}

	void Statistics::ChangeSatiety(int characterId, double delta)
	{
		ChangeCurrent(characterId, Statistic::SATIETY, delta);
	}

	void Statistics::ChangeInfamy(int characterId, double delta)
	{
		ChangeCurrent(characterId, Statistic::INFAMY, delta);
	}

	void Statistics::ChangeBrawling(int characterId, double delta)
	{
		ChangeCurrent(characterId, Statistic::BRAWLING, delta);
	}

	double Statistics::GetDignity(int characterId)
	{
		return GetCurrentWithBuffs(characterId, character::Statistic::DIGNITY);
	}

	double Statistics::GetPoshness(int characterId)
	{
		return GetCurrentWithBuffs(characterId, character::Statistic::POSHNESS);
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