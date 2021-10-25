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
	static const std::string LAYOUT_NAME = "State.Terminal";
	static const ::UIState CURRENT_STATE = ::UIState::ABOUT;
	static const std::string TERMINAL_ID = "terminal";

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "");
		visuals::Terminals::SetForeground(LAYOUT_NAME, TERMINAL_ID, game::Colors::LIGHT_CYAN);
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "About Seafarers of SPLORR!!:");
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "");
		visuals::Terminals::SetForeground(LAYOUT_NAME, TERMINAL_ID, game::Colors::DARK_GRAY);
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "A Production of TheGrumpyGameDev");
		visuals::Terminals::WriteLine(LAYOUT_NAME, TERMINAL_ID, "");
		visuals::Terminals::SetForeground(LAYOUT_NAME, TERMINAL_ID, game::Colors::GRAY);
		application::UIState::Write(::UIState::MAIN_MENU);
	}

	void About::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}
