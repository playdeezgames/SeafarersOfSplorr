#include "State.InPlay.DockOrCareen.h"
#include "State.InPlay.AtSeaOverview.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.Next.h"
#include <Game.Session.Player.h>
#include <Game.Session.Character.h>
#include <Game.Session.Character.Berth.h>
namespace state::in_play
{
	std::optional<int> DockOrCareen::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();

		auto ship = 
			game::session::character::Berth(game::session::Player::GetCharacterId())
			.GetShip();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Dock or Careen:");
		Terminal::SetForeground(game::Colors::GRAY);

		Terminal::ShowMenu();

		Terminal::ShowPrompt();
	}

	static void OnDock()
	{
		game::session::character::Berth(game::session::Player::GetCharacterId())
			.GetShip()
			.Dock();
		application::UIState::Write(Next::GetStateId());
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
		Registrar::Register(stateId, [](int currentState)
			{
				::application::OnEnter::AddHandler(currentState, OnEnter);
				::application::Renderer::SetRenderLayout(currentState, Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					currentState,
					Terminal::DoMenuInput(
						Terminal::INVALID_INPUT,
						Refresh));
			});
	}
}