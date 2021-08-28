#include <Application.OnEnter.h>
#include <Application.Update.h>
#include <Application.UIState.h>
#include <Game.h>
#include <Game.Avatar.h>
#include <Game.Avatar.Docked.h>
#include <Game.Avatar.Statistics.h>
#include <Visuals.Confirmations.h>
#include <Visuals.Messages.h>
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

	const std::map<game::avatar::State, ::UIState> dockedStateTable =
	{
		{ game::avatar::State::DARK_ALLEY, ::UIState::IN_PLAY_DARK_ALLEY },
		{ game::avatar::State::DARK_ALLEY_ENTRANCE, ::UIState::IN_PLAY_DARK_ALLEY_ENTRANCE }, 
		{ game::avatar::State::SHIPYARD, ::UIState::IN_PLAY_SHIPYARD },
		{ game::avatar::State::MARKET_BUY,::UIState::IN_PLAY_ISLAND_BUY },
		{ game::avatar::State::MARKET_SELL, ::UIState::IN_PLAY_ISLAND_SELL },
		{ game::avatar::State::MARKET, ::UIState::IN_PLAY_ISLAND_TRADE },
		{ game::avatar::State::JOB_BOARD, ::UIState::IN_PLAY_ISLAND_JOBS },
		{ game::avatar::State::GAMBLE_START, ::UIState::IN_PLAY_GAMBLE_START },
		{ game::avatar::State::AT_SEA, ::UIState::IN_PLAY_AT_SEA },
		{ game::avatar::State::CAREENED_TO_PORT, ::UIState::IN_PLAY_CAREENED_TO_PORT },
		{ game::avatar::State::CAREENED_TO_STARBOARD, ::UIState::IN_PLAY_CAREENED_TO_STARBOARD },
		{ game::avatar::State::TEMPLE, ::UIState::IN_PLAY_TEMPLE }
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
		auto dockedState = game::Avatar::GetState();
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

	static void OnUpdate(const unsigned int&)
	{
		OnEnter();
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_NEXT, OnEnter);
		::application::Update::AddHandler(::UIState::IN_PLAY_NEXT, OnUpdate);
	}
}
