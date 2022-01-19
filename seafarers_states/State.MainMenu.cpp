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
#include "State.Registrar.h"
#include "UIState.h"
namespace state
{
	std::optional<int> MainMenu::stateId = std::nullopt;

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
		{ "2", application::UIState::DoGoTo(About::GetStateId)},
		{ "3", application::UIState::GoTo(::UIState::STATISTICS)},
		{ "4", application::UIState::PushTo(::UIState::OPTIONS)},
		{ "5", application::UIState::GoTo(::UIState::CONFIRM_QUIT)}
	};

	static void DoStart(int stateId)
	{
		::application::OnEnter::AddHandler(stateId, OnEnter);
		::application::Renderer::SetRenderLayout(stateId, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			stateId, 
			Terminal::DoIntegerInput(
				menuActions, 
				Terminal::INVALID_INPUT,
				Refresh));
	}

	void MainMenu::Start()
	{
		Registrar::Register(stateId, DoStart);
	}
}