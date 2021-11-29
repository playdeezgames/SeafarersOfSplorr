#include <Application.Update.h>
#include <Game.h>
#include <Game.Character.Actions.h>
#include <Game.Character.Statistics.h>
#include <Visuals.Confirmations.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.Next.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_NEXT;

	struct StatusChecker
	{
		std::function<bool()> checker;
		::UIState destination;
	};

	static bool IsPlayerOutOfTurns()
	{
		return game::character::Statistics::IsPlayerOutOfTurns(GetPlayerCharacterId());
	}

	static bool IsPlayerDead()
	{
		return game::character::Statistics::IsPlayerDead(GetPlayerCharacterId());
	}

	static const std::list<StatusChecker> statusCheckers =
	{
		{IsPlayerOutOfTurns, ::UIState::IN_PLAY_WIN},
		{IsPlayerDead, ::UIState::IN_PLAY_LOSE},
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

		auto avatarState = game::character::Actions::GetState(GetPlayerCharacterId());
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
