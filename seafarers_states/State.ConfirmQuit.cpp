#include <Application.Command.h>
#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include "State.ConfirmQuit.h"
#include "State.Terminal.h"
#include "State.MainMenu.h"
#include "UIState.h"
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::CONFIRM_QUIT;

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);

		Terminal::ClearStatusLine();

		Terminal::ClearInput();
		Terminal::WriteLine();

		Terminal::SetForeground(game::Colors::RED);
		Terminal::WriteLine("Are you sure you want to quit?");
		Terminal::WriteLine();

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Yes");
		Terminal::WriteLine("0) No");

		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine();
		Terminal::Write(">");
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "0", application::UIState::DoGoTo(MainMenu::GetStateId)},
		{ "1", application::UIState::GoTo(::UIState::QUIT)}
	};

	void ConfirmQuit::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE, 
			Terminal::DoIntegerInput(
				menuActions, 
				Terminal::INVALID_INPUT, 
				OnEnter));
	}
}