#include "Application.Command.h"
#include "Application.OnEnter.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.Renderer.h"
#include "Application.Update.h"
#include "Common.Utility.h"
#include "Game.Audio.Mux.h"
#include "Game.Avatar.h"
#include "Game.Heading.h"
#include "Visuals.Areas.h"
#include "Visuals.Images.h"
#include "Visuals.Menus.h"
namespace state::in_play::AtSea
{
	const std::string LAYOUT_NAME = "State.InPlay.AtSea";

	const std::string MENU_ID = "Order";

	const std::string AREA_CHANGE_HEADING = "ChangeHeading";
	const std::string AREA_CHANGE_SPEED = "ChangeSpeed";
	const std::string AREA_MOVE = "Move";

	const std::string IMAGE_NEW_HEADING = "NewHeading";

	const common::XY<int> CENTER = { 64, 64 };//TODO: hardcoded

	void RefreshAvatarStatus();
	void RefreshIslands();

	const size_t TICKS_TOTAL = 1000;
	static size_t ticksLeft = TICKS_TOTAL;
	enum class AutoMoveState
	{
		OFF,
		ON,
		STARTING
	};
	static AutoMoveState autoMoveState = AutoMoveState::OFF;
	static void ToggleAutoMove()
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

	static double newHeading = 0.0;

	enum class OrderMenuItem
	{
		CHANGE_SPEED,
		HEAD_FOR,
		MOVE,
		DOCK,
		JOB,
		CARGO,
		SHIP
	};

	static void OnUpdate(const unsigned int& ticks)
	{
		if (autoMoveState != AutoMoveState::OFF)
		{
			if (ticks >= ticksLeft)
			{
				ticksLeft = TICKS_TOTAL;
				game::Avatar::Move();
				application::UIState::Write(::UIState::IN_PLAY_NEXT);
			}
			else
			{
				ticksLeft -= ticks;
			}
		}
	}

	static void OnDock()
	{
		game::Avatar::Dock();
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	const std::map<OrderMenuItem, std::function<void()>> activators =
	{
		{OrderMenuItem::CHANGE_SPEED, ::application::UIState::GoTo(::UIState::IN_PLAY_CHANGE_SPEED)},
		{OrderMenuItem::MOVE, ToggleAutoMove},
		{OrderMenuItem::DOCK, OnDock},
		{OrderMenuItem::HEAD_FOR, application::UIState::GoTo(::UIState::IN_PLAY_HEAD_FOR)},
		{OrderMenuItem::JOB, application::UIState::GoTo(::UIState::IN_PLAY_CURRENT_JOB)},
		{OrderMenuItem::CARGO, application::UIState::GoTo(::UIState::IN_PLAY_CARGO)},
		{OrderMenuItem::SHIP, application::UIState::PushTo(::UIState::IN_PLAY_SHIP_STATUS)}
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (OrderMenuItem)visuals::Menus::ReadIndex(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{::Command::GREEN, ActivateItem },
		{::Command::BACK, ::application::UIState::GoTo(::UIState::LEAVE_PLAY) },
		{::Command::RED, ::application::UIState::GoTo(::UIState::LEAVE_PLAY) }
	};

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

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		RefreshAvatarStatus();
		RefreshIslands();
	}

	static void OnMouseMotionInHelm(const common::XY<int>& location)
	{
		visuals::Images::SetVisible(LAYOUT_NAME, IMAGE_NEW_HEADING, true);
		auto delta = location - CENTER;
		newHeading = game::Heading::XYToDegrees({(double)delta.GetX(), (double)delta.GetY()});
		visuals::Images::SetAngle(LAYOUT_NAME, IMAGE_NEW_HEADING, newHeading);
	}

	static void OnMouseMotionInArea(const std::string& areaName, const common::XY<int>& location)
	{
		if (areaName == AREA_CHANGE_HEADING)
		{
			OnMouseMotionInHelm(location);
			return;
		}
		visuals::Images::SetVisible(LAYOUT_NAME, IMAGE_NEW_HEADING, false);
	}

	static void OnMouseMotionOutsideArea(const common::XY<int>& location)
	{
		visuals::Images::SetVisible(LAYOUT_NAME, IMAGE_NEW_HEADING, false);
	}

	static bool OnMouseButtonUp(const std::string& areaName)
	{
		if (areaName == AREA_CHANGE_HEADING)
		{
			game::Avatar::SetHeading(newHeading);
			RefreshAvatarStatus();
			return true;
		}
		return false;
	}

	void Start()
	{
		::application::Update::AddHandler(::UIState::IN_PLAY_AT_SEA, OnUpdate);
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_AT_SEA, OnEnter);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_AT_SEA, LAYOUT_NAME);
		::application::Command::SetHandlers(::UIState::IN_PLAY_AT_SEA, commandHandlers);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_AT_SEA, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea, OnMouseMotionOutsideArea));
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_AT_SEA, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_AT_SEA, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUp));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_AT_SEA, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
	}
}