#include "Application.OnEnter.h"
#include "Application.UIState.h"
#include "Game.h"
#include "Game.Avatar.h"
#include "Game.Avatar.Statistics.h"
namespace state::in_play::Next
{
	static void OnEnter()
	{
		if (game::avatar::Statistics::IsOutOfTurns())
		{
			application::UIState::Write(::UIState::IN_PLAY_WIN);
			return;
		}
		if (game::avatar::Statistics::GetCurrent(game::avatar::Statistic::HEALTH) == game::avatar::Statistics::GetMinimum(game::avatar::Statistic::HEALTH))
		{
			application::UIState::Write(::UIState::IN_PLAY_LOSE);
			return;
		}

		game::AutoSave();
		if (game::Avatar::GetDockedLocation())
		{
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
