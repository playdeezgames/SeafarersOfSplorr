#include "Game.Avatar.Statistics.h"
#include "Data.Game.Avatar.Statistics.h"
#include <map>
#include <list>
namespace game::avatar::Statistics
{
	struct StatisticDescriptor
	{
		game::avatar::Statistic statistic;
		double minimum;
		double maximum;
		double intial;
	};

	const std::map<game::Difficulty, std::list<StatisticDescriptor>> initialValues =
	{
		{game::Difficulty::EASY,
			{
				{game::avatar::Statistic::TURNS_REMAINING, 0.0, 10000.0, 10000.0},
				{game::avatar::Statistic::HEALTH, 0.0, 100.0, 100.0},
				{game::avatar::Statistic::SATIETY, 0.0, 100.0, 100.0},
				{game::avatar::Statistic::REPUTATION, -1000.0, 1000.0, 0.0},
				{game::avatar::Statistic::MONEY, 0.0, 1000000000.0, 100.0}
			}},
		{game::Difficulty::NORMAL,
			{
				{game::avatar::Statistic::TURNS_REMAINING, 0.0, 10000.0, 10000.0},
				{game::avatar::Statistic::HEALTH, 0.0, 100.0, 100.0},
				{game::avatar::Statistic::SATIETY, 0.0, 100.0, 100.0},
				{game::avatar::Statistic::REPUTATION, -1000.0, 1000.0, 0.0},
				{game::avatar::Statistic::MONEY, 0.0, 1000000000.0, 0.0}
			}},
		{game::Difficulty::HARD,
			{
				{game::avatar::Statistic::TURNS_REMAINING, 0.0, 10000.0, 10000.0},
				{game::avatar::Statistic::HEALTH, 0.0, 100.0, 100.0},
				{game::avatar::Statistic::SATIETY, 0.0, 100.0, 100.0},
				{game::avatar::Statistic::REPUTATION, -1000.0, 1000.0, 0.0},
				{game::avatar::Statistic::MONEY, 0.0, 1000000000.0, 0.0}
			}},
		{game::Difficulty::HARDCORE,
			{
				{game::avatar::Statistic::TURNS_REMAINING, 0.0, 10000.0, 10000.0},
				{game::avatar::Statistic::HEALTH, 0.0, 100.0, 100.0},
				{game::avatar::Statistic::SATIETY, 0.0, 100.0, 100.0},
				{game::avatar::Statistic::REPUTATION, -1000.0, 1000.0, 0.0},
				{game::avatar::Statistic::MONEY, 0.0, 1000000000.0, 0.0}
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
					value.intial
				});
		}
	}

	double GetMaximum(const game::avatar::Statistic& statistic)
	{
		return data::game::avatar::Statistics::Read((int)statistic).value().maximum;
	}

	double GetMinimum(const game::avatar::Statistic& statistic)
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
			(value > data.maximum) ? (data.maximum) :
			(value < data.minimum) ? (data.minimum) :
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

	void Eat(double amount)
	{
		double healthDown = GetMaximum(game::avatar::Statistic::HEALTH) - GetCurrent(game::avatar::Statistic::HEALTH);
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
			GetMaximum(game::avatar::Statistic::HEALTH) - GetCurrent(game::avatar::Statistic::HEALTH) +
			GetMaximum(game::avatar::Statistic::SATIETY) - GetCurrent(game::avatar::Statistic::SATIETY);
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

}