#include <Application.OnEnter.h>
#include <Application.Update.h>
#include <Application.UIState.h>
#include <Game.h>
#include <Game.Character.h>
#include <Game.Character.Actions.h>
#include <Game.Character.Docked.h>
#include <Game.Character.Statistics.h>
#include <Game.Player.h>
#include <Visuals.Confirmations.h>
#include <Visuals.Messages.h>
#include <map>
#include "State.InPlay.Next.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_NEXT;

	struct StatusChecker
	{
		std::function<bool()> checker;
		::UIState destination;
	};

	static const std::list<StatusChecker> statusCheckers =
	{
		{game::character::Statistics::IsPlayerOutOfTurns, ::UIState::IN_PLAY_WIN},
		{game::character::Statistics::IsPlayerDead, ::UIState::IN_PLAY_LOSE},
		{visuals::Messages::HasMessage, ::UIState::IN_PLAY_MESSAGE},
		{visuals::Confirmations::HasConfirmation, ::UIState::IN_PLAY_CONFIRM}
	};

	static const std::map<game::character::State, ::UIState> avatarStateTable =
	{
		{ game::character::State::DARK_ALLEY, ::UIState::IN_PLAY_DARK_ALLEY },
		{ game::character::State::DARK_ALLEY_ENTRANCE, ::UIState::IN_PLAY_DARK_ALLEY_ENTRANCE },
		{ game::character::State::SHIPYARD, ::UIState::IN_PLAY_SHIPYARD },
		{ game::character::State::MARKET_BUY,::UIState::IN_PLAY_ISLAND_BUY },
		{ game::character::State::MARKET_SELL, ::UIState::IN_PLAY_ISLAND_SELL },
		{ game::character::State::MARKET, ::UIState::IN_PLAY_ISLAND_TRADE },
		{ game::character::State::JOB_BOARD, ::UIState::IN_PLAY_ISLAND_JOBS },
		{ game::character::State::GAMBLE_START, ::UIState::IN_PLAY_GAMBLE_INTRO },
		{ game::character::State::AT_SEA, ::UIState::IN_PLAY_AT_SEA_OVERVIEW },
		{ game::character::State::CAREENED_TO_PORT, ::UIState::IN_PLAY_CAREENED_TO_PORT },
		{ game::character::State::CAREENED_TO_STARBOARD, ::UIState::IN_PLAY_CAREENED_TO_STARBOARD },
		{ game::character::State::TEMPLE, ::UIState::IN_PLAY_TEMPLE },
		{ game::character::State::FISHING, ::UIState::IN_PLAY_FISHING }
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

		if (application::UIState::CanPop())
		{
			application::UIState::Pop();
			return;
		}

		auto avatarState = game::character::Actions::GetState(game::Player::GetCharacterId());
		if (avatarState)
		{
			auto iter = avatarStateTable.find(avatarState.value());
			if(iter!= avatarStateTable.end())
			{
				application::UIState::Write(iter->second);
				return;
			}
			application::UIState::Write(::UIState::IN_PLAY_DOCKED);
			return;
		}
		application::UIState::Write(::UIState::IN_PLAY_AT_SEA_OVERVIEW);
		return;
	}

	static void OnUpdate(const unsigned int&)
	{
		OnEnter();
	}

	void Next::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Update::AddHandler(CURRENT_STATE, OnUpdate);
	}
}
