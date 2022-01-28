#include "State.InPlay.Ship.HuntRats.h"
#include "State.InPlay.Globals.h"
#include "State.Registrar.h"
#include "State.InPlay.ShipStatus.h"
namespace state::in_play::ship
{
	std::optional<int> HuntRats::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();

		//auto ship =
		//	game::session::Ship(
		//		game::session::character::Berth(game::session::Player::GetCharacterId()).GetShipId());

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Hunting Rats:");
		Terminal::SetForeground(game::Colors::GRAY);

		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static void UpdateMenu()
	{
		//auto shipId = game::session::character::Berth(game::session::Player::GetCharacterId()).GetShipId();
		//auto ship = game::session::Ship(shipId);
		//if (game::session::ship::Counter(shipId, game::ships::Counter::RATS).GetValue() > 0)
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		MenuAction defaultAction = { "Never mind", application::UIState::DoGoTo(ShipStatus::GetStateId) };
		Terminal::menu.SetDefaultAction(defaultAction);

	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void HuntRats::Start()
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