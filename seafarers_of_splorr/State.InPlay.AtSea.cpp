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
#include "Application.MouseButtonUp.h"
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

	static void OnChangeHeading()
	{
		::application::UIState::Write(::UIState::IN_PLAY_CHANGE_HEADING);
	}

	static void OnChangeSpeed()
	{

	}

	static void OnMove()
	{

	}

	const std::map<OrderMenuItem, std::function<void()>> activators =
	{
		{OrderMenuItem::CHANGE_HEADING, OnChangeHeading},
		{OrderMenuItem::CHANGE_SPEED, OnChangeSpeed},
		{OrderMenuItem::MOVE, OnMove}
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
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_X, std::format("X: {:.2f}", location.GetX()));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_Y, std::format("Y: {:.2f}", location.GetY()));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_HEADING, std::format("Heading: {:.2f}", heading));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_SPEED, std::format("Speed: {:.2f}", speed));
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_AT_SEA, OnEnter);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_AT_SEA, LAYOUT_NAME);
		::application::Command::SetHandlers(::UIState::IN_PLAY_AT_SEA, commandHandlers);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_AT_SEA, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_AT_SEA, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
	}
}