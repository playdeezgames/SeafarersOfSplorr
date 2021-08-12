#include "Application.OnEnter.h"
#include "Application.UIState.h"
#include "Game.h"
#include "Game.Avatar.h"
#include "Game.Avatar.Docked.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Messages.h"
namespace state::in_play::Next
{
	static void OnEnter()
	{
		if (game::avatar::Statistics::IsOutOfTurns())
		{
			application::UIState::Write(::UIState::IN_PLAY_WIN);
			return;
		}
		if (game::avatar::Statistics::IsDead())
		{
			application::UIState::Write(::UIState::IN_PLAY_LOSE);
			return;
		}

		game::AutoSave();
		if (!game::Messages::IsEmpty())
		{
			application::UIState::Write(::UIState::IN_PLAY_MESSAGE);
			return;
		}
		if (game::avatar::Docked::GetDockedLocation())
		{
			auto dockedState = game::avatar::Docked::GetDockedState().value();
			switch (dockedState)
			{
			case game::avatar::DockedState::DARK_ALLEY_ENTRANCE:
				application::UIState::Write(::UIState::IN_PLAY_DARK_ALLEY_ENTRANCE);
				return;
			case game::avatar::DockedState::SHIPYARD:
				application::UIState::Write(::UIState::IN_PLAY_SHIPYARD);
				return;
			case game::avatar::DockedState::MARKET_BUY:
				application::UIState::Write(::UIState::IN_PLAY_ISLAND_BUY);
				return;
			case game::avatar::DockedState::MARKET_SELL:
				application::UIState::Write(::UIState::IN_PLAY_ISLAND_SELL);
				return;
			case game::avatar::DockedState::MARKET:
				application::UIState::Write(::UIState::IN_PLAY_ISLAND_TRADE);
				return;
			case game::avatar::DockedState::JOB_BOARD:
				application::UIState::Write(::UIState::IN_PLAY_ISLAND_JOBS);
				return;
			default:
				application::UIState::Write(::UIState::IN_PLAY_DOCKED);
				return;
			}
		}
		application::UIState::Write(::UIState::IN_PLAY_AT_SEA);
		return;
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_NEXT, OnEnter);
	}
}
