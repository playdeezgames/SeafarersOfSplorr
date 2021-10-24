#include <Application.Command.h>
#include <Application.Keyboard.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.Dispatcher.h>
#include <Data.JSON.Stores.h>
#include <Game.Audio.Mux.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Menus.h>
#include <Visuals.Texts.h>
#include <Visuals.Terminals.h>
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::MAIN_MENU;
	static const std::string LAYOUT_NAME = "State.Terminal";
	static const std::string TERMINAL_ID = "terminal";

	static void GoToAbout()
	{
		::application::UIState::Write(::UIState::ABOUT);
	}

	static std::string inputBuffer = "";

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		inputBuffer = "";
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "");
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "Main Menu:");
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "1) Start");
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "2) About");
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "3) Stats");
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "4) Options");
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "5) Quit");
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "");
		visuals::Terminals::WriteText(LAYOUT_NAME, TERMINAL_ID, ">");
	}

	static const std::string KEY_RETURN = "Return";
	static const std::string KEY_1 = "1";
	static const std::string KEY_2 = "2";
	static const std::string KEY_3 = "3";
	static const std::string KEY_4 = "4";
	static const std::string KEY_5 = "5";

	static const std::map<std::string, std::function<void()>> menuActions = 
	{
		{ KEY_1, application::UIState::GoTo(::UIState::START_GAME)},
		{ KEY_2, application::UIState::GoTo(::UIState::ABOUT)},
		{ KEY_3, application::UIState::GoTo(::UIState::STATISTICS)},
		{ KEY_4, application::UIState::PushTo(::UIState::OPTIONS)},
		{ KEY_5, application::UIState::GoTo(::UIState::CONFIRM_QUIT)}
	};

	static bool OnKeyDown(const std::string& key)
	{
		if (key == KEY_1 || key == KEY_2 || key == KEY_3 || key == KEY_4 || key == KEY_5)
		{
			inputBuffer += key;
			visuals::Terminals::WriteText(LAYOUT_NAME, TERMINAL_ID, key);
			return true;
		}
		else if (key == KEY_RETURN)
		{
			visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "");
			bool result = common::utility::Dispatcher::Dispatch(menuActions, inputBuffer, true, false);
			if (result)
			{
				inputBuffer = "";
			}
			else
			{
				visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "Please enter a number between 1 and 5.");
				visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "");
				OnEnter();
			}
			return true;
		}
		return false;
	}

	void MainMenu::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
		::application::Keyboard::AddHandler(CURRENT_STATE, OnKeyDown);
	}
}