#include <Application.Renderer.h>
#include <Application.OnEnter.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include "State.About.h"
#include "State.Terminal.h"
#include "UIState.h"
#include <Visuals.Terminals.h>
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::ABOUT;

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
		application::UIState::Write(::UIState::MAIN_MENU);
	}

	void About::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
	}
}
