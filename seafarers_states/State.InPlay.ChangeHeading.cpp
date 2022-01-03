#include <Common.Heading.h>
#include <Game.Session.h>
#include "State.InPlay.ChangeHeading.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CHANGE_HEADING;

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
		if (ship.GetNearbyIslands().HasAny())
		{
			Terminal::menu.AddAction({ "Head for a nearby island", application::UIState::GoTo(::UIState::IN_PLAY_HEAD_FOR_NEAR_BY) });
		}
		Terminal::menu.AddAction({ "Set heading manually", application::UIState::GoTo(::UIState::IN_PLAY_MANUAL_HEADING) });
		MenuAction defaultAction = { "Never mind", application::UIState::GoTo(::UIState::IN_PLAY_SHIP_STATUS) };
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
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoMenuInput(
				Terminal::INVALID_INPUT,
				Refresh));
	}
}