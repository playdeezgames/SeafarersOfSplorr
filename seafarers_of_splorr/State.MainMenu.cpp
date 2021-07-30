#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.OnEnter.h"
#include "Application.Renderer.h"
#include "Application.UIState.h"
#include "Common.Audio.h"
#include "Common.Utility.h"
#include "Data.Stores.h"
#include "Game.Audio.Mux.h"
#include "Visuals.Areas.h"
#include "Visuals.Menus.h"
#include "Visuals.Texts.h"
namespace state::MainMenu
{
	const std::string LAYOUT_NAME = "State.MainMenu";
	const std::string MENU_ID = "Main";
	const std::string SUBLAYOUT_MODDED = "Sublayout.Modded";
	const std::string TEXT_MODDED = "modded";
	const std::string MODDED = "Modded";

	enum class MainMenuItem
	{
		START,
		ABOUT,
		STATISTICS,
		OPTIONS,
		QUIT
	};

	static void GoToAbout()
	{
		::application::UIState::Write(::UIState::ABOUT);
	}

	const std::map<MainMenuItem, std::function<void()>> activators =
	{
		{ MainMenuItem::START, ::application::UIState::GoTo(::UIState::START_GAME) },
		{ MainMenuItem::OPTIONS, ::application::UIState::PushTo(::UIState::OPTIONS)  },
		{ MainMenuItem::STATISTICS, ::application::UIState::GoTo(::UIState::STATISTICS)  },
		{ MainMenuItem::ABOUT, GoToAbout },
		{ MainMenuItem::QUIT, ::application::UIState::GoTo(::UIState::CONFIRM_QUIT)  },
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (MainMenuItem)visuals::Menus::ReadIndex(LAYOUT_NAME, MENU_ID).value());
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
		if (data::Stores::IsModded())
		{
			visuals::Texts::SetText(SUBLAYOUT_MODDED, TEXT_MODDED, MODDED);
		}
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::MAIN_MENU, OnEnter);
		::application::MouseMotion::AddHandler(::UIState::MAIN_MENU, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseButtonUp::AddHandler(::UIState::MAIN_MENU, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::Command::SetHandlers(::UIState::MAIN_MENU, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::MAIN_MENU, LAYOUT_NAME);
	}
}