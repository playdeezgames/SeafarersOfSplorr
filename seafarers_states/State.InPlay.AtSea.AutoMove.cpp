#include <Application.UIState.h>
#include <Game.Avatar.AtSea.h>
#include <Game.Islands.h>
#include "UIState.h"
namespace state::in_play::AtSea
{
	const size_t TICKS_TOTAL = 250;
	static size_t ticksLeft = TICKS_TOTAL;
	enum class AutoMoveState
	{
		OFF,
		ON,
		STARTING
	};
	static AutoMoveState autoMoveState = AutoMoveState::OFF;
	bool IsAutoMoveEngaged()
	{
		return autoMoveState != AutoMoveState::OFF;
	}
	void ToggleAutoMove()
	{
		if (autoMoveState == AutoMoveState::OFF)
		{
			ticksLeft = TICKS_TOTAL;
			autoMoveState = (game::Islands::CanDock()) ? (AutoMoveState::STARTING) : (AutoMoveState::ON);
		}
		else
		{
			autoMoveState = AutoMoveState::OFF;
		}
	}

	void DoAutomoveTimer(const unsigned int& ticks)
	{
		if (autoMoveState != AutoMoveState::OFF)
		{
			if (ticks >= ticksLeft)
			{
				ticksLeft = TICKS_TOTAL;
				if (game::avatar::AtSea::Move() == game::avatar::AtSea::MoveResult::CLAMPED)
				{
					autoMoveState = AutoMoveState::OFF;
				}
				application::UIState::Write(::UIState::IN_PLAY_NEXT);
			}
			else
			{
				ticksLeft -= ticks;
			}
		}
	}

	void UpdateAutoMoveState()
	{
		if (game::Islands::CanDock())
		{
			if (autoMoveState == AutoMoveState::ON)
			{
				autoMoveState = AutoMoveState::OFF;
			}
		}
		else
		{
			if (autoMoveState == AutoMoveState::STARTING)
			{
				autoMoveState = AutoMoveState::ON;
			}
		}
	}
}