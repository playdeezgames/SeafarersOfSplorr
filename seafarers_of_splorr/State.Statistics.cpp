#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.OnEnter.h"
#include "Game.Audio.Mux.h"
#include "Game.Achievements.h"
#include "Visuals.Texts.h"
#include <format>
#include "Common.Utility.h"
#include <tuple>
namespace state::Statistics
{
	const std::string LAYOUT_NAME = "State.Statistics";
	const std::string TEXT_MOVES_MADE = "MovesMade";
	const std::string TEXT_BEERS_DRUNK = "BeersDrunk";
	const std::string TEXT_BOOZE_DRUNK = "BoozeDrunk";
	const std::string TEXT_COFFEE_DRUNK = "CoffeeDrunk";
	const std::string TEXT_DIED_WITHOUT_DIGNITY = "DiedWithoutDignity";
	const std::string TEXT_DIED_WITH_DIGNITY = "DiedWithDignity";
	const std::string TEXT_FOOD_EATEN = "FoodEaten";
	const std::string TEXT_GAMES_PLAYED = "GamesPlayed";
	const std::string TEXT_GOT_DRUNK_STAT = "GotDrunkStat";
	const std::string TEXT_KEYS_USED = "KeysUsed";
	const std::string TEXT_MAP_FRAGMENTS_FOUND = "MapFragmentsFound";
	const std::string TEXT_SOILED_TROUSERS = "SoiledTrousers";
	const std::string TEXT_TIMES_DIED = "TimesDied";
	const std::string TEXT_TIMES_DROPPED_TROU = "TimesDroppedTrou";
	const std::string TEXT_TIMES_POOPED = "TimesPooped";
	const std::string TEXT_WALLS_BUMPED = "WallsBumped";
	const std::string TEXT_WINE_DRUNK = "WineDrunk";
	const std::string TEXT_KILLED_BY_WEREWOLF = "KilledByWerewolf";
	const std::string TEXT_KILLED_BY_WYVERN = "KilledByWyvern";
	const std::string TEXT_KILLED_BY_BRAIN_TENTACLE = "KilledByBrainTentacle";
	const std::string TEXT_KILLED_BY_IMP = "KilledByImp";
	const std::string TEXT_KILLED_BY_TROGLODYTE = "KilledByTroglodyte";
	const std::string TEXT_KILLED_BY_BULLY_MINION = "KilledByBullyMinion";
	const std::string TEXT_KILLED_BY_EVIL_BAT = "KilledByEvilBat";
	const std::string TEXT_KILLED_BY_EVIL_MINION = "KilledByEvilMinion";
	const std::string TEXT_BROKEN_WALL = "BrokenWall";


	static bool OnMouseButtonUp(const common::XY<int>& xy, unsigned char buttons)
	{
		::application::UIState::Write(::UIState::MAIN_MENU);
		return true;
	}

	static void SetStatisticText(const std::string& textId, const std::string& caption, const game::Statistic& statistic)
	{
		auto value = game::Statistics::Read(statistic);
		if (value)
		{
			visuals::Texts::SetText(LAYOUT_NAME, textId, std::format("{}: {}", caption, value.value()));
		}
		else
		{
			visuals::Texts::SetText(LAYOUT_NAME, textId, std::format("{}: -", caption));
		}
	}

	const std::vector<std::tuple<std::string, std::string, game::Statistic>> statisticsList =
	{
		{ TEXT_MOVES_MADE, "Moves Made", game::Statistic::MOVES_MADE},
		{ TEXT_GAMES_PLAYED, "Games Played", game::Statistic::GAMES_PLAYED}
	};

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		for (auto& item : statisticsList)
		{
			SetStatisticText(std::get<0>(item), std::get<1>(item), std::get<2>(item));
		}
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::STATISTICS, OnEnter);
		::application::MouseButtonUp::AddHandler(::UIState::STATISTICS, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::STATISTICS, ::application::UIState::GoTo(::UIState::MAIN_MENU));
		::application::Renderer::SetRenderLayout(::UIState::STATISTICS, LAYOUT_NAME);
	}
}
