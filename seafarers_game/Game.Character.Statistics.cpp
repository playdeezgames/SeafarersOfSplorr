#include <Common.Utility.h>
#include <Common.Utility.Optional.h>
#include <Common.Utility.Table.h>
#include <Data.Game.Character.Statistic.h>
#include "Game.Characters.Equipment.h"
#include "Game.Characters.Statistic.h"
#include "Game.Characters.Statistics.h"
#include "Game.Item.h"
namespace game::character
{
	struct StatisticDescriptor
	{
		game::character::Statistic statistic;
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

	typedef std::map<game::character::Statistic, BuffTable> StatBuffTables;

	static const StatBuffTables allBuffs =
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

	void Statistics::Reset(int characterId, const game::Difficulty& difficulty)
	{
		auto values = initialValues.find(difficulty)->second;
		for (auto& value : values)
		{
			data::game::character::Statistic::Write(
				characterId,
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
		auto stat = data::game::character::Statistic::Read(characterId, (int)statistic);
		if (stat)
		{
			return stat.value().maximum;
		}
		return std::nullopt;
	}

	static std::optional<double> GetMinimum(int characterId, const game::character::Statistic& statistic)
	{
		auto stat = data::game::character::Statistic::Read(characterId, (int)statistic);
		if (stat)
		{
			return stat.value().minimum;
		}
		return std::nullopt;
	}

	static std::optional<double> GetCurrent(int characterId, const game::character::Statistic& statistic)
	{
		auto stat = data::game::character::Statistic::Read(characterId, (int)statistic);
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
				result += (game::character::Equipment::IsEquipped(characterId, item)) ? (buff) : (0.0);
			});
	}

	static std::optional<double> GetCurrentWithBuffs(int characterId, const game::character::Statistic& statistic)
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

	static void SetCurrent(int characterId, const game::character::Statistic& statistic, double value)
	{
		auto data = data::game::character::Statistic::Read(characterId, (int)statistic).value();
		data.current = common::Utility::Clamp<double>(value, data.minimum, data.maximum);
		data::game::character::Statistic::Write(
			characterId,
			(int)statistic, 
			data);
	}

	static std::optional<double> ChangeCurrent(int characterId, const game::character::Statistic& statistic, double delta)
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
		std::optional<double> Money::Current(int characterId)
		{
			return GetCurrent(characterId, game::character::Statistic::MONEY);
		}

		std::optional<double> Money::Change(int characterId, double delta)
		{
			return ChangeCurrent(characterId, game::character::Statistic::MONEY, delta);
		}

		std::optional<double> Health::Current(int characterId)
		{
			return GetCurrent(characterId, game::character::Statistic::HEALTH);
		}

		std::optional<double> Health::Change(int characterId, double delta)
		{
			return ChangeCurrent(characterId, game::character::Statistic::HEALTH, delta);
		}

		std::optional<bool> Health::IsDead(int characterId)
		{
			return IsMinimal(characterId, Statistic::HEALTH);
		}

		std::optional<double> Satiety::Current(int characterId)
		{
			return GetCurrent(characterId, game::character::Statistic::SATIETY);
		}

		std::optional<double> Satiety::Change(int characterId, double delta)
		{
			return ChangeCurrent(characterId, game::character::Statistic::SATIETY, delta);
		}

		std::optional<bool> Satiety::IsStarving(int characterId)
		{
			return IsMinimal(characterId, game::character::Statistic::SATIETY);
		}

		void Satiety::Eat(int characterId, double amount)
		{
			auto healthDown = GetDownAmount(characterId, Statistic::HEALTH);
			if (healthDown)
			{
				ChangeCurrent(characterId, Statistic::HEALTH, amount);
				amount -= healthDown.value();

			}
			if (amount > 0)
			{
				auto satietyDown = GetDownAmount(characterId, Statistic::SATIETY);
				if (satietyDown)
				{
					ChangeCurrent(characterId, Statistic::SATIETY, amount);
				}
			}
		}

		std::optional<double> Satiety::NeedsToEat(int characterId, double amount)
		{
			auto downHealth = GetDownAmount(characterId, game::character::Statistic::HEALTH);
			if (downHealth)
			{
				auto downSatiety = GetDownAmount(characterId, game::character::Statistic::SATIETY);
				if (downSatiety)
				{
					return (downHealth.value() + downSatiety.value()) >= amount;
				}
			}
			return std::nullopt;
		}
		std::optional<double> Reputation::Current(int characterId)
		{
			return GetCurrent(characterId, game::character::Statistic::REPUTATION);
		}

		std::optional<double> Reputation::Change(int characterId, double delta)
		{
			return ChangeCurrent(characterId, game::character::Statistic::REPUTATION, delta);
		}

		std::optional<double> Infamy::Current(int characterId)
		{
			return GetCurrent(characterId, game::character::Statistic::INFAMY);
		}

		std::optional<double> Infamy::Change(int characterId, double delta)
		{
			return ChangeCurrent(characterId, game::character::Statistic::INFAMY, delta);
		}

		std::optional<double> Brawling::Current(int characterId)
		{
			return GetCurrent(characterId, game::character::Statistic::BRAWLING);
		}

		std::optional<double> Brawling::Change(int characterId, double delta)
		{
			return ChangeCurrent(characterId, game::character::Statistic::BRAWLING, delta);
		}

		std::optional<int> Turns::Remaining(int characterId)
		{
			return common::utility::Optional::Map<double, int>(
				GetCurrent(characterId, game::character::Statistic::TURNS_REMAINING),
				common::Utility::Cast<double, int>);
		}

		std::optional<int> Turns::Change(int characterId, int amount)
		{
			return common::utility::Optional::Map<double, int>(
				ChangeCurrent(characterId, game::character::Statistic::TURNS_REMAINING, (double)amount),
				common::Utility::Cast<double, int>);
		}

		std::optional<bool> Turns::HasRemaining(int characterId)
		{
			auto turns = Remaining(characterId);
			if (turns)
			{
				return turns.value() > 0;
			}
			return std::nullopt;
		}

		std::optional<double> Dignity::Current(int characterId)
		{
			return GetCurrentWithBuffs(characterId, character::Statistic::DIGNITY);
		}

		std::optional<double> Poshness::Current(int characterId)
		{
			return GetCurrentWithBuffs(characterId, character::Statistic::POSHNESS);
		}
	}
}