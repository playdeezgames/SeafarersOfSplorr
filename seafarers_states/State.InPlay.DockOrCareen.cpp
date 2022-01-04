#include <Game.Session.h>
#include "State.InPlay.DockOrCareen.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_DOCK_OR_CAREEN;

	static void Refresh()
	{
		Terminal::Reinitialize();

		auto ship = 
			game::Session()
			.GetPlayer().GetCharacter()
			.GetBerth()
			.GetShip();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Dock or Careen:");
		Terminal::SetForeground(game::Colors::GRAY);

		Terminal::ShowMenu();

		Terminal::ShowPrompt();
	}

	static void OnDock()
	{
		game::Session()
			.GetPlayer()
			.GetCharacter()
			.GetBerth()
			.GetShip()
			.Dock();
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		Terminal::menu.AddAction({ "Dock", OnDock });
		MenuAction defaultAction = { "Never mind", application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA_OVERVIEW) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void DockOrCareen::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoMenuInput(
				Terminal::INVALID_INPUT,
				Refresh));
	}
}