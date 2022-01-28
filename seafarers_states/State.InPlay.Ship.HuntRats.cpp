#include "State.InPlay.Ship.HuntRats.h"
#include "State.InPlay.Globals.h"
#include "State.Registrar.h"
#include "State.InPlay.ShipStatus.h"
#include <Game.Session.Character.h>
#include <Game.Session.Player.h>
#include <Game.Session.Ship.Counter.h>
namespace state::in_play::ship
{
	std::optional<int> HuntRats::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Hunting Rats:");
		Terminal::SetForeground(game::Colors::GRAY);
		auto shipId = game::session::Character(game::session::Player::GetCharacterId()).GetShipId();
		auto counterValue = game::session::ship::Counter(shipId, game::ships::Counter::RATS).GetValue();
		Terminal::WriteLine("There are {} rats on board.", counterValue);
		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static void HuntRat()
	{
		//TODO: the stuff
		Refresh();
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		auto shipId = game::session::Character(game::session::Player::GetCharacterId()).GetShipId();
		auto counterValue = game::session::ship::Counter(shipId, game::ships::Counter::RATS).GetValue();
		if (counterValue > 0)
		{
			Terminal::menu.AddAction({ "Hunt!", HuntRat });
		}
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