#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.ManualHeading.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_MANUAL_HEADING;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("What is yer new heading?");
		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		PlayMainTheme();
		Refresh();
	}

	static void OnValidInput(double value)
	{
		game::Session().GetPlayer().GetCharacter().GetBerth().GetShip().SetHeading(value);
		application::UIState::Write(::UIState::IN_PLAY_SHIP_STATUS);
	}

	void ManualHeading::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoDoubleInput(
				application::UIState::GoTo(::UIState::IN_PLAY_SHIP_STATUS),
				OnValidInput));
	}
}