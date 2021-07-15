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
				{game::avatar::Statistic::HUNGER, 0.0, 100.0, 100.0},
				{game::avatar::Statistic::REPUTATION, -1000.0, 1000.0, 0.0},
				{game::avatar::Statistic::MONEY, 0.0, 1000000000.0, 0.0}
			}},
		{game::Difficulty::NORMAL,
			{
				{game::avatar::Statistic::TURNS_REMAINING, 0.0, 10000.0, 10000.0},
				{game::avatar::Statistic::HEALTH, 0.0, 100.0, 100.0},
				{game::avatar::Statistic::HUNGER, 0.0, 100.0, 100.0},
				{game::avatar::Statistic::REPUTATION, -1000.0, 1000.0, 0.0},
				{game::avatar::Statistic::MONEY, 0.0, 1000000000.0, 0.0}
			}},
		{game::Difficulty::HARD,
			{
				{game::avatar::Statistic::TURNS_REMAINING, 0.0, 10000.0, 10000.0},
				{game::avatar::Statistic::HEALTH, 0.0, 100.0, 100.0},
				{game::avatar::Statistic::HUNGER, 0.0, 100.0, 100.0},
				{game::avatar::Statistic::REPUTATION, -1000.0, 1000.0, 0.0},
				{game::avatar::Statistic::MONEY, 0.0, 1000000000.0, 0.0}
			}},
		{game::Difficulty::HARDCORE,
			{
				{game::avatar::Statistic::TURNS_REMAINING, 0.0, 10000.0, 10000.0},
				{game::avatar::Statistic::HEALTH, 0.0, 100.0, 100.0},
				{game::avatar::Statistic::HUNGER, 0.0, 100.0, 100.0},
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
}