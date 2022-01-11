#include <Application.Update.h>
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
		return 
			game::Session()
			.GetPlayer()
			.GetCharacter()
			.GetCounters()
			.GetCounter(game::characters::Counter::TURNS_REMAINING)
			.GetValue() <= 0;
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

		auto character = game::Session().GetPlayer().GetCharacter();
		auto island = character.TryGetIsland();
		if (island)
		{
			application::UIState::Write(::UIState::IN_PLAY_DOCKED);
		}
		else
		{
			application::UIState::Write(::UIState::IN_PLAY_AT_SEA_OVERVIEW);
		}
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
