#include "Application.Command.h"
#include "Application.OnEnter.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.Renderer.h"
#include "Application.UIState.h"
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

	const std::string MENU_ITEM_MOVE = "Move";

	const common::XY<int> CENTER = { 160, 160 };//TODO: hardcoded

	void RefreshAvatarStatus();
	bool RefreshIslands();
	void DoAutomoveTimer(const unsigned int&);
	void ToggleAutoMove();
	bool IsAutoMoveEngaged();
	void UpdateAutoMoveState(bool);

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

	const std::string STOP_MOVE = "Stop Move";
	const std::string START_MOVE = "Start Move";

	static void RefreshMoveMenuItem()
	{
		visuals::MenuItems::SetText(
			LAYOUT_NAME,
			MENU_ITEM_MOVE,
			(IsAutoMoveEngaged()) ?
			(STOP_MOVE) :
			(START_MOVE));
	}

	static void OnUpdate(const unsigned int& ticks)
	{
		DoAutomoveTimer(ticks);
		RefreshMoveMenuItem();
	}

	static void OnDock()
	{
		if (game::Avatar::Dock() == game::Avatar::DockResult::COMPLETED_QUEST)
		{
			application::UIState::Write(::UIState::IN_PLAY_COMPLETED_JOB);
			return;
		}
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

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		RefreshAvatarStatus();
		UpdateAutoMoveState(RefreshIslands());
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