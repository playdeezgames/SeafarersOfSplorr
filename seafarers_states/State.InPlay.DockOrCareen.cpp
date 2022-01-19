#include <Game.Session.h>
#include "State.InPlay.AtSeaOverview.h"
#include "State.InPlay.DockOrCareen.h"
#include "State.InPlay.Globals.h"
#include "State.Registrar.h"
namespace state::in_play
{
	std::optional<int> DockOrCareen::stateId = std::nullopt;

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
		MenuAction defaultAction = 
			{ 
				"Never mind", 
				application::UIState::DoGoTo(AtSeaOverview::GetStateId) 
			};
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
		Registrar::Register(stateId, [](int stateId)
			{
				::application::OnEnter::AddHandler(stateId, OnEnter);
				::application::Renderer::SetRenderLayout(stateId, Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					stateId,
					Terminal::DoMenuInput(
						Terminal::INVALID_INPUT,
						Refresh));
			});
	}
}