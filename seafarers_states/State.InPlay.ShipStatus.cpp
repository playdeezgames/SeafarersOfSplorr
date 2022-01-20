#include <format>
#include <Game.Session.h>
#include "State.InPlay.AtSeaOverview.h"
#include "State.InPlay.RenameShipAdjective.h"
#include "State.InPlay.ChangeHeading.h"
#include "State.InPlay.ChangeSpeed.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.MarkCurrentLocation.h"
#include "State.InPlay.ShipStatus.h"
#include "State.Registrar.h"
namespace state::in_play
{
	std::optional<int> ShipStatus::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();

		auto ship = game::Session().GetPlayer().GetCharacter().GetBerth().GetShip();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Ship status:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Name: {}", ship.GetName());

		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static void UpdateMenu()
	{
		auto ship = game::Session().GetPlayer().GetCharacter().GetBerth().GetShip();
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		Terminal::menu.AddAction({ std::format("Change Heading(Current: {:.2f}\xf8)", ship.GetHeading()), application::UIState::DoGoTo(ChangeHeading::GetStateId) });
		Terminal::menu.AddAction({ std::format("Change Speed(Current: {:.1f})", ship.GetSpeed()), application::UIState::DoGoTo(ChangeSpeed::GetStateId) });
		Terminal::menu.AddAction({ "Mark Current Location", application::UIState::DoGoTo(MarkCurrentLocation::GetStateId) });
		Terminal::menu.AddAction({ "Rename ship", application::UIState::DoGoTo(RenameShipAdjective::GetStateId) });
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