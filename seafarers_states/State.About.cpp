#include <Application.Renderer.h>
#include <Application.OnEnter.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include "State.About.h"
#include "State.Registrar.h"
#include "State.Terminal.h"
#include "State.MainMenu.h"
#include <Visuals.Terminals.h>
namespace state
{
	std::optional<int> About::stateId = std::nullopt;

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Terminal::WriteLine();
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("About Seafarers of SPLORR!!:");
		Terminal::WriteLine();
		Terminal::SetForeground(game::Colors::DARK_GRAY);
		Terminal::WriteLine("A Production of TheGrumpyGameDev");
		Terminal::WriteLine();
		Terminal::SetForeground(game::Colors::GRAY);
		application::UIState::Write(MainMenu::GetStateId());
	}

	static void DoStart(int state)
	{
		::application::OnEnter::AddHandler(state, OnEnter);
		::application::Renderer::SetRenderLayout(state, Terminal::LAYOUT_NAME);
	}

	void About::Start()
	{
		Registrar::Register(stateId, DoStart);
	}
}
