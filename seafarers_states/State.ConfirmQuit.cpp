#include <Application.Command.h>
#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include "State.Terminal.h"
#include "States.h"
#include "UIState.h"
#include <Visuals.Terminals.h>
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::CONFIRM_QUIT;

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);

		visuals::Terminals::WriteLine(Terminal::LAYOUT_NAME, Terminal::STATUS_ID, "");

		Terminal::ClearInput();
		visuals::Terminals::WriteLine(Terminal::LAYOUT_NAME, Terminal::TERMINAL_ID, "");

		visuals::Terminals::SetForeground(Terminal::LAYOUT_NAME, Terminal::TERMINAL_ID, game::Colors::RED);
		visuals::Terminals::WriteLine(Terminal::LAYOUT_NAME, Terminal::TERMINAL_ID, "Are you sure you want to quit?");
		visuals::Terminals::WriteLine(Terminal::LAYOUT_NAME, Terminal::TERMINAL_ID, "");

		visuals::Terminals::SetForeground(Terminal::LAYOUT_NAME, Terminal::TERMINAL_ID, game::Colors::YELLOW);
		visuals::Terminals::WriteLine(Terminal::LAYOUT_NAME, Terminal::TERMINAL_ID, "1) No");
		visuals::Terminals::WriteLine(Terminal::LAYOUT_NAME, Terminal::TERMINAL_ID, "2) Yes");
		visuals::Terminals::SetForeground(Terminal::LAYOUT_NAME, Terminal::TERMINAL_ID, game::Colors::GRAY);

		visuals::Terminals::WriteLine(Terminal::LAYOUT_NAME, Terminal::TERMINAL_ID, "");

		visuals::Terminals::WriteText(Terminal::LAYOUT_NAME, Terminal::TERMINAL_ID, ">");
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "1", application::UIState::GoTo(::UIState::MAIN_MENU)},
		{ "2", application::UIState::GoTo(::UIState::QUIT)}
	};

	void ConfirmQuit::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(CURRENT_STATE, Terminal::DoIntegerInput(menuActions, "Please enter a number between 1 and 2.", OnEnter));
	}
}