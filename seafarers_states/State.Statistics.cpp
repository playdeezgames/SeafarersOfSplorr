#include <Application.Renderer.h>
#include <Application.OnEnter.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include "State.MainMenu.h"
#include "State.Registrar.h"
#include "State.Statistics.h"
#include "State.Terminal.h"
namespace state
{
	std::optional<int> Statistics::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::ClearStatusLine();
		Terminal::WriteLine();
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Statistics:");
		Terminal::WriteLine();
		Terminal::SetForeground(game::Colors::DARK_GRAY);
		Terminal::WriteLine("TODO: put statistics here!");
		application::UIState::Write(MainMenu::GetStateId());
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	void Statistics::Start()
	{
		Registrar::Register(stateId, [](int currentState)
			{
				::application::OnEnter::AddHandler(currentState, OnEnter);
				::application::Renderer::SetRenderLayout(currentState, Terminal::LAYOUT_NAME);
			});
	}
}
