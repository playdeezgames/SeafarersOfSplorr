#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.Renderer.h"
#include "Common.Utility.h"
#include "Visuals.Areas.h"
#include "Visuals.Menus.h"
#include "Application.OnEnter.h"
#include "Common.Audio.h"
#include "Game.Audio.Mux.h"
#include "Visuals.Texts.h"
#include "Data.Stores.h"
#include "Game.Avatar.h"
namespace state::in_play::Docked
{
	const std::string LAYOUT_NAME = "State.InPlay.Docked";
	const std::string MENU_ID = "Order";

	enum class OrderMenuItem
	{
		UNDOCK
	};

	static void DoUndock()
	{
		game::Avatar::Undock();
		application::UIState::EnterGame();
	}

	const std::map<OrderMenuItem, std::function<void()>> activators =
	{
		{ OrderMenuItem::UNDOCK, DoUndock }
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
		{::Command::BACK, ::application::UIState::GoTo(::UIState::CONFIRM_QUIT) },
		{::Command::RED, ::application::UIState::GoTo(::UIState::CONFIRM_QUIT) }
	};

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_DOCKED, OnEnter);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_DOCKED, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_DOCKED, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::Command::SetHandlers(::UIState::IN_PLAY_DOCKED, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_DOCKED, LAYOUT_NAME);
	}
}