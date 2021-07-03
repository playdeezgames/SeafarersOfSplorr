#include <string>
#include "Application.OnEnter.h"
#include "Game.Audio.Mux.h"
#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Game.Avatar.h"
#include "Visuals.Texts.h"
#include <format>
#include "Visuals.Menus.h"
#include "Common.Utility.h"
#include "Visuals.Areas.h"
#include "Application.MouseMotion.h"
namespace state::in_play::AtSea 
{ 
	const std::string LAYOUT_NAME = "State.InPlay.AtSea";
	const std::string TEXT_AVATAR_X = "avatar-x";
	const std::string TEXT_AVATAR_Y = "avatar-y";
	const std::string TEXT_AVATAR_HEADING = "avatar-heading";
	const std::string TEXT_AVATAR_SPEED = "avatar-speed";
	const std::string MENU_ID = "Order";
	const std::string AREA_CHANGE_HEADING = "ChangeHeading";
	const std::string AREA_CHANGE_SPEED = "ChangeSpeed";
	const std::string AREA_MOVE = "Move";

	enum class OrderMenuItem
	{
		CHANGE_HEADING,
		CHANGE_SPEED,
		MOVE
	};

	const std::map<OrderMenuItem, std::function<void()>> activators =
	{
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

	void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		auto location = game::Avatar::GetLocation();
		auto heading = game::Avatar::GetHeading();
		auto speed = game::Avatar::GetSpeed();
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_X, std::format("X: {}", location.GetX()));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_Y, std::format("Y: {}", location.GetY()));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_HEADING, std::format("Heading: {}", heading));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_SPEED, std::format("Speed: {}", speed));
	}

	const std::map<std::string, OrderMenuItem> areaMenuItems =
	{
		{ AREA_CHANGE_HEADING,  OrderMenuItem::CHANGE_HEADING},
		{ AREA_CHANGE_SPEED,  OrderMenuItem::CHANGE_SPEED},
		{ AREA_MOVE,  OrderMenuItem::MOVE}
	};

	static void SetCurrentMenuItem(OrderMenuItem item)
	{
		visuals::Menus::WriteIndex(LAYOUT_NAME, MENU_ID, (int)item);
	}

	static void OnMouseMotionInArea(const std::string& area, const common::XY<int>&)
	{
		SetCurrentMenuItem(areaMenuItems.find(area)->second);
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_AT_SEA, OnEnter);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_AT_SEA, LAYOUT_NAME);
		::application::Command::SetHandlers(::UIState::IN_PLAY_AT_SEA, commandHandlers);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_AT_SEA, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea));
	}
}