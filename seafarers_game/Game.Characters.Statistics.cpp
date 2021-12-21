#include <Common.Utility.h>
#include <Common.Utility.Optional.h>
#include <Common.Utility.Table.h>
#include <Data.Game.Character.StatisticLegacy.h>
#include "Game.Characters.Equipment.h"
#include "Game.Characters.Statistic.h"
#include "Game.Characters.Statistics.h"
#include "Game.ItemLegacy.h"
#include "Game.Session.h"
namespace game::characters
{
	struct StatisticDescriptor
	{
		game::characters::Statistic statistic;
		std::optional<double> minimum;
		std::optional<double> maximum;
		double initial;
	};

	typedef std::map<Item, double> BuffTable;

	static const BuffTable dignityBuffs =
	{
		{ Item::TROUSERS, 100.0 },
		{ Item::POSH_TROUSERS, 100.0 }
	};

	static const BuffTable poshBuffs =
	{
		{ Item::POSH_TROUSERS, 100.0 }
	};

	typedef std::map<game::characters::Statistic, BuffTable> StatBuffTables;

	static const StatBuffTables allBuffs =
	{
	};

	static const std::map<game::Difficulty, std::list<StatisticDescriptor>> initialValues =
	{
		{game::Difficulty::EASY,
			{
				{game::characters::Statistic::REPUTATION, std::nullopt, std::nullopt, 0.0},
				{game::characters::Statistic::BRAWLING, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::characters::Statistic::INFAMY, std::optional<double>(0.0), std::nullopt, 0.0}
			}},
		{game::Difficulty::NORMAL,
			{
				{game::characters::Statistic::REPUTATION, std::nullopt, std::nullopt, 0.0},
				{game::characters::Statistic::BRAWLING, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::characters::Statistic::INFAMY, std::optional<double>(0.0), std::nullopt, 0.0}
			}},
		{game::Difficulty::HARD,
			{
				{game::characters::Statistic::REPUTATION, std::nullopt, std::nullopt, 0.0},
				{game::characters::Statistic::BRAWLING, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::characters::Statistic::INFAMY, std::optional<double>(0.0), std::nullopt, 0.0}
			}},
		{game::Difficulty::HARDCORE,
			{
				{game::characters::Statistic::REPUTATION, std::nullopt, std::nullopt, 0.0},
				{game::characters::Statistic::BRAWLING, std::optional<double>(0.0), std::nullopt, 0.0},
				{game::characters::Statistic::INFAMY, std::optional<double>(0.0), std::nullopt, 0.0}
			}}
	};

	void Statistics::Reset(int characterId, const game::Difficulty& difficulty)
	{
		auto values = initialValues.find(difficulty)->second;
		for (auto& value : values)
		{
			data::game::character::StatisticLegacy::Write(
				characterId,
				(int)value.statistic,
				{
					value.minimum,
					value.maximum,
					value.initial
				});
		}
	}

	static std::optional<double> GetMaximum(int characterId, const game::characters::Statistic& statistic)
	{
		auto stat = data::game::character::StatisticLegacy::Read(characterId, (int)statistic);
		if (stat)
		{
			return stat.value().maximum;
		}
		return std::nullopt;
	}

	static std::optional<double> GetMinimum(int characterId, const game::characters::Statistic& statistic)
	{
		auto stat = data::game::character::StatisticLegacy::Read(characterId, (int)statistic);
		if (stat)
		{
			return stat.value().minimum;
		}
		return std::nullopt;
	}

	static std::optional<double> GetCurrent(int characterId, const game::characters::Statistic& statistic)
	{
		auto stat = data::game::character::StatisticLegacy::Read(characterId, (int)statistic);
		if (stat)
		{
			return stat.value().current;
		}
		return std::nullopt;
	}

	static double CalculateBuffs(int characterId, const BuffTable itemBuffs)
	{
		return common::utility::Table::Accumulate<game::Item, double, double>(
			itemBuffs,
			[characterId](double& result, const game::Item& item, const double& buff)
			{
				result += (game::characters::Equipment::IsEquipped(characterId, item)) ? (buff) : (0.0);
			});
	}

	static std::optional<double> GetCurrentWithBuffs(int characterId, const game::characters::Statistic& statistic)
	{
		auto current = GetCurrent(characterId, statistic);
		if (current)
		{
			auto result = current.value();
			auto buffTable = common::utility::Table::TryGetKey(allBuffs, statistic);
			if (buffTable)
			{
				result += CalculateBuffs(characterId, buffTable.value());
			}
			return result;
		}
		return std::nullopt;
	}

	static void SetCurrent(int characterId, const game::characters::Statistic& statistic, double value)
	{
		auto data = data::game::character::StatisticLegacy::Read(characterId, (int)statistic).value();
		data.current = common::Utility::Clamp<double>(value, data.minimum, data.maximum);
		data::game::character::StatisticLegacy::Write(
			characterId,
			(int)statistic, 
			data);
	}

	static std::optional<double> ChangeCurrent(int characterId, const game::characters::Statistic& statistic, double delta)
	{
		auto current = GetCurrent(characterId, statistic);
		if (current)
		{
			SetCurrent(characterId, statistic, current.value() + delta);
		}
		return GetCurrent(characterId, statistic);
	}

	static std::optional<bool> IsMinimal(int characterId, const Statistic& statistic)
	{
		auto current = GetCurrent(characterId, statistic);
		if (current)
		{
			auto minimum = GetMinimum(characterId, statistic);
			if (minimum)
			{
				return current.value() <= minimum.value();
			}
			return false;
		}
		return std::nullopt;
	}

	static std::optional<double> GetDownAmount(int characterId, const Statistic& statistic)
	{
		auto maximum = GetMaximum(characterId, statistic);
		if (maximum)
		{
			auto current = GetCurrent(characterId, statistic);
			if (current)
			{
				return maximum.value() - current.value();
			}
		}
		return std::nullopt;
	}

	namespace statistics
	{
		std::optional<double> Reputation::Current(int characterId)
		{
			return GetCurrent(characterId, game::characters::Statistic::REPUTATION);
		}

		std::optional<double> Reputation::Change(int characterId, double delta)
		{
			return ChangeCurrent(characterId, game::characters::Statistic::REPUTATION, delta);
		}

		std::optional<double> Infamy::Current(int characterId)
		{
			return GetCurrent(characterId, game::characters::Statistic::INFAMY);
		}

		std::optional<double> Infamy::Change(int characterId, double delta)
		{
			return ChangeCurrent(characterId, game::characters::Statistic::INFAMY, delta);
		}

		std::optional<double> Brawling::Current(int characterId)
		{
			return GetCurrent(characterId, game::characters::Statistic::BRAWLING);
		}

		std::optional<double> Brawling::Change(int characterId, double delta)
		{
			return ChangeCurrent(characterId, game::characters::Statistic::BRAWLING, delta);
		}
	}
}