#include "Application.UIState.h"
#include "Game.Avatar.h"
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
			autoMoveState = AutoMoveState::STARTING;
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
				if (game::Avatar::Move() == game::Avatar::MoveResult::CLAMPED)
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

	void UpdateAutoMoveState(bool canDock)
	{
		if (canDock)
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