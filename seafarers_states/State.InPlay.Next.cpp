#include "Application.OnEnter.h"
#include "Application.UIState.h"
#include "Game.h"
#include "Game.Avatar.Docked.h"
#include "Game.Avatar.Statistics.h"
#include "Visuals.Confirmations.h"
#include "Visuals.Messages.h"
#include <map>
#include "UIState.h"
namespace state::in_play::Next
{
	struct StatusChecker
	{
		std::function<bool()> checker;
		::UIState destination;
	};

	const std::list<StatusChecker> statusCheckers =
	{
		{game::avatar::Statistics::IsOutOfTurns, ::UIState::IN_PLAY_WIN},
		{game::avatar::Statistics::IsDead, ::UIState::IN_PLAY_LOSE},
		{visuals::Messages::HasMessage, ::UIState::IN_PLAY_MESSAGE},
		{visuals::Confirmations::HasConfirmation, ::UIState::IN_PLAY_CONFIRM}
	};

	const std::map<game::avatar::DockedState, ::UIState> dockedStateTable =
	{
		{ game::avatar::DockedState::DARK_ALLEY, ::UIState::IN_PLAY_DARK_ALLEY },
		{ game::avatar::DockedState::DARK_ALLEY_ENTRANCE, ::UIState::IN_PLAY_DARK_ALLEY_ENTRANCE }, 
		{ game::avatar::DockedState::SHIPYARD, ::UIState::IN_PLAY_SHIPYARD },
		{ game::avatar::DockedState::MARKET_BUY,::UIState::IN_PLAY_ISLAND_BUY },
		{ game::avatar::DockedState::MARKET_SELL, ::UIState::IN_PLAY_ISLAND_SELL },
		{ game::avatar::DockedState::MARKET, ::UIState::IN_PLAY_ISLAND_TRADE },
		{ game::avatar::DockedState::JOB_BOARD, ::UIState::IN_PLAY_ISLAND_JOBS },
		{ game::avatar::DockedState::GAMBLE_START, ::UIState::IN_PLAY_GAMBLE_START }
	};

	static void OnEnter()
	{
		for (auto statusChecker : statusCheckers)
		{
			if (statusChecker.checker())
			{
				application::UIState::Write(statusChecker.destination);
				return;
			}
		}
		game::AutoSave();
		auto dockedState = game::avatar::Docked::GetDockedState();
		if (dockedState)
		{
			auto iter = dockedStateTable.find(dockedState.value());
			if(iter!=dockedStateTable.end())
			{
				application::UIState::Write(iter->second);
				return;
			}
			application::UIState::Write(::UIState::IN_PLAY_DOCKED);
			return;
		}
		application::UIState::Write(::UIState::IN_PLAY_AT_SEA);
		return;
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_NEXT, OnEnter);
	}
}