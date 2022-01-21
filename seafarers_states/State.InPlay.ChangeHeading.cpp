#include <Common.Heading.h>
#include <Game.Session.Player.h>
#include "State.InPlay.ChangeHeading.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.HeadForKnown.h"
#include "State.InPlay.HeadForMark.h"
#include "State.InPlay.HeadForNearBy.h"
#include "State.InPlay.ManualHeading.h"
#include "State.InPlay.ShipStatus.h"
namespace state::in_play
{
	std::optional<int> ChangeHeading::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();

		auto playerCharacter =
			game::session::Player().GetCharacter();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Change Heading:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Current: {:.2f}\xf8", 
			playerCharacter
			.GetBerth()
			.GetShip()
			.GetHeading());

		Terminal::ShowMenu();

		Terminal::ShowPrompt();
	}

	static void UpdateMenu()
	{
		auto playerCharacter =
			game::session::Player().GetCharacter();
		auto ship = playerCharacter.GetBerth().GetShip();
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		if (playerCharacter.GetKnownIslands().HasAny())
		{
			Terminal::menu.AddAction({ "Head for a known island", application::UIState::DoGoTo(HeadForKnown::GetStateId) });
		}
		if (playerCharacter.GetMarks().HasAny())
		{
			Terminal::menu.AddAction({ "Head for a marked location", application::UIState::DoGoTo(HeadForMark::GetStateId) });
		}
		if (ship.GetNearbyIslands().HasAny())
		{
			Terminal::menu.AddAction({ "Head for a nearby island", application::UIState::DoGoTo(HeadForNearBy::GetStateId) });
		}
		Terminal::menu.AddAction({ "Set heading manually", application::UIState::DoGoTo(ManualHeading::GetStateId) });
		MenuAction defaultAction = { "Never mind", application::UIState::DoGoTo(ShipStatus::GetStateId) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void ChangeHeading::Start()
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