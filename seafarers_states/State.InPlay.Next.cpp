#include <Application.Update.h>
#include <Game.Characters.h>
#include <Game.Characters.Statistics.h>
#include <Game.Session.h>
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
		return !game::characters::statistics::Turns::HasRemaining(GetPlayerCharacterId()).value();
	}

	static bool IsPlayerDead()
	{
		return game::Session().GetPlayer().GetCharacter().IsDead();
	}

	static const std::list<StatusChecker> statusCheckers =
	{
		{IsPlayerOutOfTurns, ::UIState::IN_PLAY_WIN},
		{IsPlayerDead, ::UIState::IN_PLAY_LOSE}
	};

	static const std::map<game::characters::State, ::UIState> avatarStateTable =
	{
		{ game::characters::State::DARK_ALLEY, ::UIState::IN_PLAY_DARK_ALLEY },
		{ game::characters::State::DARK_ALLEY_ENTRANCE, ::UIState::IN_PLAY_DARK_ALLEY_ENTRANCE },
		{ game::characters::State::SHIPYARD, ::UIState::IN_PLAY_SHIPYARD },
		{ game::characters::State::MARKET_BUY,::UIState::IN_PLAY_ISLAND_BUY },
		{ game::characters::State::MARKET_SELL, ::UIState::IN_PLAY_ISLAND_SELL },
		{ game::characters::State::MARKET, ::UIState::IN_PLAY_ISLAND_TRADE },
		{ game::characters::State::JOB_BOARD, ::UIState::IN_PLAY_ISLAND_JOBS },
		{ game::characters::State::GAMBLE_START, ::UIState::IN_PLAY_GAMBLE_INTRO },
		{ game::characters::State::AT_SEA, ::UIState::IN_PLAY_AT_SEA_OVERVIEW },
		{ game::characters::State::CAREENED_TO_PORT, ::UIState::IN_PLAY_CAREENED_TO_PORT },
		{ game::characters::State::CAREENED_TO_STARBOARD, ::UIState::IN_PLAY_CAREENED_TO_STARBOARD },
		{ game::characters::State::TEMPLE, ::UIState::IN_PLAY_TEMPLE },
		{ game::characters::State::FISHING, ::UIState::IN_PLAY_FISHING },
		{ game::characters::State::TAVERN, ::UIState::IN_PLAY_TAVERN }
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
		game::Session().GetSaves().GetAuto().Store();

		if (application::UIState::CanPop())
		{
			application::UIState::Pop();
			return;
		}

		auto characterState = game::Characters::GetState(GetPlayerCharacterId());
		if (characterState)
		{
			auto iter = avatarStateTable.find(characterState.value());
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
