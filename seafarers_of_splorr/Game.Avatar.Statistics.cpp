#include "Game.Avatar.Statistic.h"
#include "Game.Avatar.Statistics.h"
#include "Data.Game.Avatar.Statistics.h"
#include <map>
#include <list>
namespace game::avatar::Statistics
{
	struct StatisticDescriptor
	{
		game::avatar::Statistic statistic;
		std::optional<double> minimum;
		std::optional<double> maximum;
		double initial;
	};

	const std::map<game::Difficulty, std::list<StatisticDescriptor>> initialValues =
	{
		{game::Difficulty::EASY,
			{
				{game::avatar::Statistic::TURNS_REMAINING, std::optional<double>(0.0), std::optional<double>(10000.0), 10000.0},
				{game::avatar::Statistic::HEALTH, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::avatar::Statistic::SATIETY, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::avatar::Statistic::REPUTATION, std::nullopt, std::nullopt, 0.0},
				{game::avatar::Statistic::MONEY, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::avatar::Statistic::BRAWLING, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::avatar::Statistic::INFAMY, std::optional<double>(0.0), std::nullopt, 0.0}
			}},
		{game::Difficulty::NORMAL,
			{
				{game::avatar::Statistic::TURNS_REMAINING, std::optional<double>(0.0), std::optional<double>(10000.0), 10000.0},
				{game::avatar::Statistic::HEALTH, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::avatar::Statistic::SATIETY, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::avatar::Statistic::REPUTATION, std::nullopt, std::nullopt, 0.0},
				{game::avatar::Statistic::MONEY, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::avatar::Statistic::BRAWLING, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::avatar::Statistic::INFAMY, std::optional<double>(0.0), std::nullopt, 0.0}
			}},
		{game::Difficulty::HARD,
			{
				{game::avatar::Statistic::TURNS_REMAINING, std::optional<double>(0.0), std::optional<double>(10000.0), 10000.0},
				{game::avatar::Statistic::HEALTH, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::avatar::Statistic::SATIETY, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::avatar::Statistic::REPUTATION, std::nullopt, std::nullopt, 0.0},
				{game::avatar::Statistic::MONEY, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::avatar::Statistic::BRAWLING, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::avatar::Statistic::INFAMY, std::optional<double>(0.0), std::nullopt, 0.0}
			}},
		{game::Difficulty::HARDCORE,
			{
				{game::avatar::Statistic::TURNS_REMAINING, std::optional<double>(0.0), std::optional<double>(10000.0), 10000.0},
				{game::avatar::Statistic::HEALTH, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::avatar::Statistic::SATIETY, std::optional<double>(0.0), std::optional<double>(100.0), 100.0},
				{game::avatar::Statistic::REPUTATION, std::nullopt, std::nullopt, 0.0},
				{game::avatar::Statistic::MONEY, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::avatar::Statistic::BRAWLING, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::avatar::Statistic::INFAMY, std::optional<double>(0.0), std::nullopt, 0.0}
			}}
	};

	void Reset(const game::Difficulty& difficulty)
	{
		auto values = initialValues.find(difficulty)->second;
		for (auto& value : values)
		{
			data::game::avatar::Statistics::Write((int)value.statistic,
				{
					value.minimum,
					value.maximum,
					value.initial
				});
		}
	}

	static std::optional<double> GetMaximum(const game::avatar::Statistic& statistic)
	{
		return data::game::avatar::Statistics::Read((int)statistic).value().maximum;
	}

	static std::optional<double> GetMinimum(const game::avatar::Statistic& statistic)
	{
		return data::game::avatar::Statistics::Read((int)statistic).value().minimum;
	}

	double GetCurrent(const game::avatar::Statistic& statistic)
	{
		return data::game::avatar::Statistics::Read((int)statistic).value().current;
	}

	static void SetCurrent(const game::avatar::Statistic& statistic, double value)
	{
		auto data = data::game::avatar::Statistics::Read((int)statistic).value();
		data.current =
			(data.maximum.has_value() && value > data.maximum.value()) ? (data.maximum.value()) :
			(data.minimum.has_value() && value < data.minimum.value()) ? (data.minimum.value()) :
			(value);
		data::game::avatar::Statistics::Write((int)statistic, data);
	}

	double ChangeCurrent(const game::avatar::Statistic& statistic, double delta)
	{
		SetCurrent(statistic, GetCurrent(statistic) + delta);
		return GetCurrent(statistic);
	}

	double GetMoney()
	{
		return GetCurrent(game::avatar::Statistic::MONEY);
	}

	double GetHealth()
	{
		return GetCurrent(game::avatar::Statistic::HEALTH);
	}

	double GetSatiety()
	{
		return GetCurrent(game::avatar::Statistic::SATIETY);
	}

	double GetInfamy()
	{
		return GetCurrent(game::avatar::Statistic::INFAMY);
	}

	double GetBrawling()
	{
		return GetCurrent(game::avatar::Statistic::BRAWLING);
	}

	void Eat(double amount)
	{
		double healthDown = GetMaximum(game::avatar::Statistic::HEALTH).value() - GetCurrent(game::avatar::Statistic::HEALTH);
		if (healthDown > 0)
		{
			ChangeCurrent(game::avatar::Statistic::HEALTH, amount);
			amount -= healthDown;
		}
		if (amount > 0)
		{
			ChangeCurrent(game::avatar::Statistic::SATIETY, amount);
		}
	}

	bool NeedToEat(double amount)
	{
		double totalDown =
			GetMaximum(game::avatar::Statistic::HEALTH).value() - GetCurrent(game::avatar::Statistic::HEALTH) +
			GetMaximum(game::avatar::Statistic::SATIETY).value() - GetCurrent(game::avatar::Statistic::SATIETY);
		return totalDown >= amount;
	}

	double GetReputation()
	{
		return GetCurrent(game::avatar::Statistic::REPUTATION);
	}

	void ChangeReputation(double delta)
	{
		ChangeCurrent(game::avatar::Statistic::REPUTATION, delta);
	}

	int GetTurnsRemaining()
	{
		return (int)GetCurrent(game::avatar::Statistic::TURNS_REMAINING);
	}

	void SpendTurn()
	{
		const double TURN_DELTA = -1.0;
		ChangeCurrent(game::avatar::Statistic::TURNS_REMAINING, TURN_DELTA);
	}

	bool IsOutOfTurns()
	{
		return GetCurrent(game::avatar::Statistic::TURNS_REMAINING) <= GetMinimum(game::avatar::Statistic::TURNS_REMAINING);
	}

	static bool IsMinimal(const Statistic& statistic)
	{
		return GetCurrent(statistic) <= GetMinimum(statistic);
	}

	bool IsDead()
	{
		return IsMinimal(Statistic::HEALTH);
	}

	bool IsStarving()
	{
		return IsMinimal(Statistic::SATIETY);
	}

	void ChangeMoney(double delta)
	{
		ChangeCurrent(Statistic::MONEY, delta);
	}

	void ChangeHealth(double delta)
	{
		ChangeCurrent(Statistic::HEALTH, delta);
	}

	void ChangeSatiety(double delta)
	{
		ChangeCurrent(Statistic::SATIETY, delta);
	}
}