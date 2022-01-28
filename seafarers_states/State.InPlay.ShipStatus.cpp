#include "State.InPlay.ShipStatus.h"
#include "State.InPlay.AtSeaOverview.h"
#include "State.InPlay.ChangeHeading.h"
#include "State.InPlay.ChangeSpeed.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.MarkCurrentLocation.h"
#include "State.InPlay.RenameShipAdjective.h"
#include <Game.Session.Player.h>
#include <Game.Session.Character.h>
#include <Game.Session.Character.Berth.h>
#include <Game.Session.Ship.h>
#include <Game.Session.Ship.Berths.h>
#include "State.InPlay.Ship.HuntRats.h"
namespace state::in_play
{
	std::optional<int> ShipStatus::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();

		auto ship = 
			game::session::Ship(
			game::session::character::Berth(game::session::Player::GetCharacterId()).GetShipId());

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Ship status:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Name: {}", ship.GetName());

		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static void UpdateMenu()
	{
		auto shipId = game::session::character::Berth(game::session::Player::GetCharacterId()).GetShipId();
		auto ship = game::session::Ship(shipId);
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		Terminal::menu.AddAction({ std::format("Change Heading(Current: {:.2f}\xf8)", ship.GetHeading()), application::UIState::DoGoTo(ChangeHeading::GetStateId) });
		Terminal::menu.AddAction({ std::format("Change Speed(Current: {:.1f})", ship.GetSpeed()), application::UIState::DoGoTo(ChangeSpeed::GetStateId) });
		Terminal::menu.AddAction({ "Mark Current Location", application::UIState::DoGoTo(MarkCurrentLocation::GetStateId) });
		Terminal::menu.AddAction({ "Rename ship", application::UIState::DoGoTo(RenameShipAdjective::GetStateId) });
		if (game::session::ship::Berths(shipId).HasAny(game::BerthType::VERMIN))
		{
			Terminal::menu.AddAction({ "Hunt rats", application::UIState::DoGoTo(state::in_play::ship::HuntRats::GetStateId) });
		}
		MenuAction defaultAction = { "Never mind", application::UIState::DoGoTo(AtSeaOverview::GetStateId) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void ShipStatus::Start()
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