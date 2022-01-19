#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.Dispatcher.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include "State.About.h"
#include "State.MainMenu.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::MAIN_MENU;

	static void GoToAbout()
	{
		::application::UIState::Write(About::GetStateId());
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Main Menu:");
		Terminal::WriteLine();

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Start");
		Terminal::WriteLine("2) About");
		Terminal::WriteLine("3) Stats");
		Terminal::WriteLine("4) Options");
		Terminal::WriteLine("5) Quit");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions = 
	{
		{ "1", application::UIState::GoTo(::UIState::START_GAME)},
		{ "2", application::UIState::GoTo(About::GetStateId)},
		{ "3", application::UIState::GoTo(::UIState::STATISTICS)},
		{ "4", application::UIState::PushTo(::UIState::OPTIONS)},
		{ "5", application::UIState::GoTo(::UIState::CONFIRM_QUIT)}
	};

	void MainMenu::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE, 
			Terminal::DoIntegerInput(
				menuActions, 
				Terminal::INVALID_INPUT,
				Refresh));
	}
}