#include <Application.Update.h>
#include <Game.Session.h>
#include "State.InPlay.AtSeaOverview.h"
#include "State.InPlay.Docked.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.Lose.h"
#include "State.InPlay.Next.h"
#include "State.InPlay.Win.h"
#include "State.Registrar.h"
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
		Registrar::Register(stateId, [](int stateId) 
			{
				::application::OnEnter::AddHandler(stateId, OnEnter);
				::application::Update::AddHandler(stateId, OnUpdate);
			});
	}
}
