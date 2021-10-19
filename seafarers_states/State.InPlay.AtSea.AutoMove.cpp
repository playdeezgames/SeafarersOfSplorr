#include <Application.UIState.h>
#include <Common.Utility.h>
#include <format>
#include <Game.Avatar.Log.h>
#include <Game.Colors.h>
#include <Game.Islands.h>
#include <Game.Ship.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Messages.h>
namespace state::in_play
{
	enum class AutoMoveState
	{
		OFF,
		ON,
		STARTING
	};

	static const size_t TICKS_TOTAL = 250;
	static size_t ticksLeft = TICKS_TOTAL;
	static AutoMoveState autoMoveState = AutoMoveState::OFF;

	bool AtSea::IsAutoMoveEngaged()
	{
		return autoMoveState != AutoMoveState::OFF;
	}

	static void TurnAutoMoveOn()
	{
			game::avatar::Log::Write(
				game::Colors::GRAY,
				"Steady as she goes!");
			ticksLeft = TICKS_TOTAL;
			autoMoveState = (game::Islands::CanDock()) ? (AutoMoveState::STARTING) : (AutoMoveState::ON);
	}

	static void TurnAutoMoveOff()
	{

		game::avatar::Log::Write({
			game::Colors::GRAY,
			"All stop!" });
		autoMoveState = AutoMoveState::OFF;
	}

	static const std::map<bool, std::function<void()>> togglers =
	{
		{ true, TurnAutoMoveOn},
		{ false, TurnAutoMoveOff}
	};

	void AtSea::ToggleAutoMove()
	{
		common::utility::Dispatcher::Dispatch(togglers, autoMoveState == AutoMoveState::OFF);
	}

	static void RunDownTimer(const unsigned int& ticks)
	{
		if (ticks >= ticksLeft)
		{
			ticksLeft = TICKS_TOTAL;
			if (game::Ship::Move() == game::Ship::MoveResult::CLAMPED)
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

	void AtSea::DoAutomoveTimer(const unsigned int& ticks)
	{
		if (autoMoveState != AutoMoveState::OFF)
		{
			RunDownTimer(ticks);
		}
	}

	static void AddArrivalMessage()
	{
		auto island = game::Islands::GetDockableIslands().front();
		std::string message = (island.visits.has_value()) ? (std::format("You arrive at {}.", island.name)) : ("You arrive at an unknown island.");
		visuals::Messages::Write({
			"==ARRIVAL!==",
			{
				{
					{19,10},
					message,
					game::Colors::GRAY,
					visuals::HorizontalAlignment::CENTER
				}
			}
		});
	}

	void AtSea::UpdateAutoMoveState()
	{
		if (game::Islands::CanDock())
		{
			if (autoMoveState == AutoMoveState::ON)
			{
				game::avatar::Log::Write(
					game::Colors::GRAY,
					"Land ho!");
				autoMoveState = AutoMoveState::OFF;
				AddArrivalMessage();
				application::UIState::Write(::UIState::IN_PLAY_NEXT);
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