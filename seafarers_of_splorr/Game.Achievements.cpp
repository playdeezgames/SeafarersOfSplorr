#define _CRT_SECURE_NO_WARNINGS
#include "Game.Achievements.h"
#include <map>
#include <string>
#include <steam_api.h>
#include <memory>
namespace game::Achievements
{
	static ISteamUser* GetSteamUser()
	{
		static bool initialized = false;
		static ISteamUser* steamUser;
		if (!initialized)
		{
			steamUser = SteamUser();
			initialized = true;
		}
		return steamUser;
	}

	static ISteamUserStats* GetSteamUserStats()
	{
		static bool initialized = false;
		static ISteamUserStats* steamUserStats;
		if (!initialized)
		{
			steamUserStats = SteamUserStats();
			initialized = true;
		}
		return steamUserStats;
	}

	const std::map<game::Achievement, std::string> achievementNames =
	{
		{game::Achievement::YER_DEAD, "YER_DEAD"},
		{game::Achievement::POOPED_YER_TROUSERS, "POOPED_YER_TROUSERS"},
		{game::Achievement::YOU_WIN, "YOU_WIN"},
		{game::Achievement::DISTRACTED_WEREWOLF, "DISTRACTED_WEREWOLF"},
		{game::Achievement::FLASHED_WYVERN, "FLASHED_WYVERN"},
		{game::Achievement::SMITTEN, "SMITTEN"},
		{game::Achievement::GOT_DRUNK, "GOT_DRUNK"},
		{game::Achievement::HEAD_BANGER, "HEAD_BANGER"},
		{game::Achievement::FOURTH_WALL, "FOURTH_WALL"},
		{game::Achievement::DEATH_BY_WEREWOLF, "DEATH_BY_WEREWOLF"},
		{game::Achievement::DEATH_BY_WYVERN, "DEATH_BY_WYVERN"},
		{game::Achievement::DEATH_BY_BRAIN_TENTACLE, "DEATH_BY_BRAIN_TENTACLE"},
		{game::Achievement::DEATH_BY_IMP, "DEATH_BY_IMP"},
		{game::Achievement::DEATH_BY_TROGLODYTE, "DEATH_BY_TROGLODYTE"},
		{game::Achievement::DEATH_BY_BULLY_MINION, "DEATH_BY_BULLY_MINION"},
		{game::Achievement::DEATH_BY_EVIL_BAT, "DEATH_BY_EVIL_BAT"},
		{game::Achievement::DEATH_BY_EVIL_MINION, "DEATH_BY_EVIL_MINION"}

	};

	void Add(const game::Achievement& achievement)
	{
		if (GetSteamUserStats())
		{
			GetSteamUserStats()->SetAchievement(achievementNames.find(achievement)->second.c_str());
			GetSteamUserStats()->StoreStats();
		}
	}

	const uint32 APP_ID = 1655480;

	static void DoFinish()
	{
		SteamAPI_Shutdown();
	}

	bool ShouldRestartApp()
	{
#ifndef _DEBUG
		if (SteamAPI_RestartAppIfNecessary(APP_ID))
		{
			return true;
		}
#endif
		return false;
	}

	void Start()
	{
		if (SteamAPI_Init())
		{
			atexit(DoFinish);

			if (SteamUser()->BLoggedOn())
			{
				GetSteamUser();
				if (GetSteamUserStats())
				{
					GetSteamUserStats()->RequestCurrentStats();
				}
			}
		}
	}
}
namespace game::Statistics
{
	const std::map<game::Statistic, std::string> statisticsName =
	{
		{ game::Statistic::GAMES_PLAYED, "GAMES_PLAYED" },
		{ game::Statistic::MOVES_MADE, "MOVES_MADE" },
		{ game::Statistic::TIMES_POOPED, "TIMES_POOPED" },
		{ game::Statistic::TIMES_DIED,"TIMES_DIED" },
		{ game::Statistic::BEERS_DRUNK,"BEERS_DRUNK" },
		{ game::Statistic::COFFEE_DRUNK,"COFFEE_DRUNK" },
		{ game::Statistic::TIMES_DROPPED_TROU,"TIMES_DROPPED_TROU" },
		{ game::Statistic::WINE_DRUNK,"WINE_DRUNK" },
		{ game::Statistic::FOOD_EATEN,"FOOD_EATEN" },
		{ game::Statistic::KEYS_USED,"KEYS_USED" },
		{ game::Statistic::MAP_FRAGMENTS_FOUND,"MAP_FRAGMENTS_FOUND" },
		{ game::Statistic::WALLS_BUMPED,"WALLS_BUMPED" },
		{ game::Statistic::SOILED_TROUSERS,"SOILED_TROUSERS" },
		{ game::Statistic::BOOZE_DRUNK,"BOOZE_DRUNK" },
		{ game::Statistic::DIED_WITH_DIGNITY,"DIED_WITH_DIGNITY" },
		{ game::Statistic::DIED_WITHOUT_DIGNITY, "DIED_WITHOUT_DIGNITY" },
		{ game::Statistic::GOT_DRUNK_STAT, "GOT_DRUNK_STAT" },
		{ game::Statistic::KILLED_BY_WEREWOLF, "KILLED_BY_WEREWOLF" },
		{ game::Statistic::KILLED_BY_WYVERN, "KILLED_BY_WYVERN" },
		{ game::Statistic::KILLED_BY_BRAIN_TENTACLE, "KILLED_BY_BRAIN_TENTACLE" },
		{ game::Statistic::KILLED_BY_IMP, "KILLED_BY_IMP" },
		{ game::Statistic::KILLED_BY_TROGLODYTE, "KILLED_BY_TROGLODYTE" },
		{ game::Statistic::KILLED_BY_BULLY_MINION, "KILLED_BY_BULLY_MINION" },
		{ game::Statistic::KILLED_BY_EVIL_BAT, "KILLED_BY_EVIL_BAT" },
		{ game::Statistic::KILLED_BY_EVIL_MINION, "KILLED_BY_EVIL_MINION" },
		{ game::Statistic::BROKEN_WALL, "BROKEN_WALL" }
	};

	void Increment(const game::Statistic& statistic)
	{
		int32 value;
		auto& statisticName = statisticsName.find(statistic)->second;
		if (game::Achievements::GetSteamUserStats() && 
			game::Achievements::GetSteamUserStats() -> GetStat(statisticName.c_str(), &value))
		{
			value++;
			if (game::Achievements::GetSteamUserStats()->SetStat(statisticName.c_str(), value))
			{
				game::Achievements::GetSteamUserStats()->StoreStats();
			}
		}
	}

	std::optional<int> Read(const game::Statistic& statistic)
	{
		int32 value;
		auto& statisticName = statisticsName.find(statistic)->second;
		if (game::Achievements::GetSteamUserStats() &&
			game::Achievements::GetSteamUserStats()->GetStat(statisticName.c_str(), &value))
		{
			return value;
		}
		return std::nullopt;
	}

}
