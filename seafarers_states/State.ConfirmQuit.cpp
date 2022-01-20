#include <Application.Command.h>
#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include "State.ConfirmQuit.h"
#include "State.MainMenu.h"
#include "State.Registrar.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state
{
	std::optional<int> ConfirmQuit::stateId = std::nullopt;

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
		{ "1", application::UIState::GoTo((int)::UIState::QUIT)}
	};

	void ConfirmQuit::Start()
	{
		Registrar::Register(stateId, [](int stateId) 
			{
				::application::OnEnter::AddHandler(stateId, OnEnter);
				::application::Renderer::SetRenderLayout(stateId, Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					stateId,
					Terminal::DoIntegerInput(
						menuActions, 
						Terminal::INVALID_INPUT, 
						OnEnter));
			});
	}
}