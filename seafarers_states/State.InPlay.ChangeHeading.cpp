#include <Common.Heading.h>
#include <Game.Session.Player.h>
#include "State.InPlay.ChangeHeading.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.HeadForKnown.h"
#include "State.InPlay.HeadForMark.h"
#include "State.InPlay.HeadForNearBy.h"
#include "State.InPlay.ManualHeading.h"
#include "State.InPlay.ShipStatus.h"
#include <Game.Session.Character.h>
#include <Game.Session.Character.Marks.h>
#include <Game.Session.Character.Berth.h>
namespace state::in_play
{
	std::optional<int> ChangeHeading::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Change Heading:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Current: {:.2f}\xf8", 
			game::session::character::Berth(game::session::Player::GetCharacterId())
			.GetShip()
			.GetHeading());

		Terminal::ShowMenu();

		Terminal::ShowPrompt();
	}

	static void UpdateMenu()
	{
		auto playerCharacter =
			game::session::Character(game::session::Player::GetCharacterId());
		auto ship = game::session::character::Berth(playerCharacter.ToId()).GetShip();
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		if (playerCharacter.GetKnownIslands().HasAny())
		{
			Terminal::menu.AddAction({ "Head for a known island", application::UIState::DoGoTo(HeadForKnown::GetStateId) });
		}
		if (game::session::character::Marks(game::session::Player::GetCharacterId()).HasAny())
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