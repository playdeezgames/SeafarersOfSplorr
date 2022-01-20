#include <Common.Heading.h>
#include <Game.Session.h>
#include "State.InPlay.ChangeHeading.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.ShipStatus.h"
#include "State.Registrar.h"
namespace state::in_play
{
	std::optional<int> ChangeHeading::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();

		auto playerCharacter =
			game::Session()
			.GetPlayer().GetCharacter();

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
			game::Session()
			.GetPlayer().GetCharacter();
		auto ship = playerCharacter.GetBerth().GetShip();
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		if (playerCharacter.GetKnownIslands().HasAny())
		{
			Terminal::menu.AddAction({ "Head for a known island", application::UIState::GoTo(::UIState::IN_PLAY_HEAD_FOR_KNOWN) });
		}
		if (playerCharacter.GetMarks().HasAny())
		{
			Terminal::menu.AddAction({ "Head for a marked location", application::UIState::GoTo(::UIState::IN_PLAY_HEAD_FOR_MARK) });
		}
		if (ship.GetNearbyIslands().HasAny())
		{
			Terminal::menu.AddAction({ "Head for a nearby island", application::UIState::GoTo(::UIState::IN_PLAY_HEAD_FOR_NEAR_BY) });
		}
		Terminal::menu.AddAction({ "Set heading manually", application::UIState::GoTo(::UIState::IN_PLAY_MANUAL_HEADING) });
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