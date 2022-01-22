#include "State.InPlay.Next.h"
#include "State.InPlay.AtSeaOverview.h"
#include "State.InPlay.Docked.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.Lose.h"
#include "State.InPlay.Win.h"
#include <Application.Update.h>
#include <Game.Session.Player.h>
#include <Game.Session.Saves.h>
#include <Game.Session.Character.h>
#include <Game.Session.Character.Counters.h>
namespace state::in_play
{
	std::optional<int> Next::stateId = std::nullopt;

	struct StatusChecker
	{
		std::function<bool()> checker;
		std::function<int()> destination;
	};

	static bool IsPlayerOutOfTurns()
	{
		return 
			game::session::character::Counters(game::session::Player::GetCharacterId())
			.GetCounter(game::characters::Counter::TURNS_REMAINING)
			.GetValue() <= 0;
	}

	static bool IsPlayerDead()
	{
		return game::session::Character(game::session::Player::GetCharacterId()).IsDead();
	}

	static const std::list<StatusChecker> statusCheckers =
	{
		{IsPlayerOutOfTurns, Win::GetStateId },
		{IsPlayerDead, Lose::GetStateId }
	};

	static void OnEnter()
	{
		for (auto statusChecker : statusCheckers)
		{
			if (statusChecker.checker())
			{
				application::UIState::Write(statusChecker.destination());
				return;
			}
		}
		game::session::Saves().GetAuto().Store();

		if (application::UIState::CanPop())
		{
			application::UIState::Pop();
			return;
		}

		auto character = game::session::Character(game::session::Player::GetCharacterId());
		auto island = character.TryGetIsland();
		if (island)
		{
			application::UIState::Write(Docked::GetStateId());
		}
		else
		{
			application::UIState::Write(AtSeaOverview::GetStateId());
		}
	}

	static void OnUpdate(const unsigned int&)
	{
		OnEnter();
	}

	void Next::Start()
	{
		Registrar::Register(stateId, [](int currentState)
			{
				::application::OnEnter::AddHandler(currentState, OnEnter);
				::application::Update::AddHandler(currentState, OnUpdate);
			});
	}
}
