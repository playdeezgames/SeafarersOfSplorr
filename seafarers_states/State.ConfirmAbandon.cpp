#include <Application.Command.h>
#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include "State.ConfirmAbandon.h"
#include "State.LeavePlay.h"
#include "State.Registrar.h"
#include "State.Terminal.h"
#include "State.MainMenu.h"
namespace state
{
	std::optional<int> ConfirmAbandon::stateId{};

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);

		Terminal::ClearStatusLine();

		Terminal::ClearInput();
		Terminal::WriteLine();

		Terminal::SetForeground(game::Colors::RED);
		Terminal::WriteLine("Are you sure you want to abandon yer current session?");
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
		{ "0", application::UIState::DoGoTo(LeavePlay::GetStateId)},
		{ "1", application::UIState::DoGoTo(MainMenu::GetStateId)}
	};

	void ConfirmAbandon::Start()
	{
		Registrar::Register(stateId, [](int stateId) 
			{
				::application::OnEnter::AddHandler(
					stateId, 
					OnEnter);
				::application::Renderer::SetRenderLayout(
					stateId, 
					Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					stateId, 
					Terminal::DoIntegerInput(
						menuActions, 
						Terminal::INVALID_INPUT, 
						OnEnter));
			});
	}
}