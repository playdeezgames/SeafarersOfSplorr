#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.Dispatcher.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Terminals.h>
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::MAIN_MENU;
	static const std::string LAYOUT_NAME = "State.Terminal";
	static const std::string TERMINAL_ID = "terminal";
	static const std::string STATUS_ID = "status";

	static void GoToAbout()
	{
		::application::UIState::Write(::UIState::ABOUT);
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);

		visuals::Terminals::WriteLine(LAYOUT_NAME, STATUS_ID, "");

		Terminal::ClearInput();
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "");

		visuals::Terminals::SetForeground(LAYOUT_NAME, TERMINAL_ID, game::Colors::LIGHT_CYAN);
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "Main Menu:");
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "");

		visuals::Terminals::SetForeground(LAYOUT_NAME, TERMINAL_ID, game::Colors::YELLOW);
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "1) Start");
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "2) About");
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "3) Stats");
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "4) Options");
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "5) Quit");
		visuals::Terminals::SetForeground(LAYOUT_NAME, TERMINAL_ID, game::Colors::GRAY);

		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "");


		visuals::Terminals::WriteText(LAYOUT_NAME, TERMINAL_ID, ">");
	}

	static const std::map<std::string, std::function<void()>> menuActions = 
	{
		{ "1", application::UIState::GoTo(::UIState::START_GAME)},
		{ "2", application::UIState::GoTo(::UIState::ABOUT)},
		{ "3", application::UIState::GoTo(::UIState::STATISTICS)},
		{ "4", application::UIState::PushTo(::UIState::OPTIONS)},
		{ "5", application::UIState::GoTo(::UIState::CONFIRM_QUIT)}
	};

	void MainMenu::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
		::application::Keyboard::AddHandler(CURRENT_STATE, Terminal::DoOnKeyDown(menuActions, "Please enter a number between 1 and 5.", OnEnter));
	}
}