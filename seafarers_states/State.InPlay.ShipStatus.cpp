#include <format>
#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.ShipStatus.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_SHIP_STATUS;

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
		Terminal::menu.AddAction({ std::format("Change Heading(Current: {:.2f}\xf8)", ship.GetHeading()), application::UIState::GoTo(::UIState::IN_PLAY_CHANGE_HEADING) });
		Terminal::menu.AddAction({ std::format("Change Speed(Current: {:.1f})", ship.GetSpeed()), application::UIState::GoTo(::UIState::IN_PLAY_CHANGE_SPEED) });
		Terminal::menu.AddAction({ "Mark Current Location", application::UIState::GoTo(::UIState::IN_PLAY_MARK_LOCATION) });
		Terminal::menu.AddAction({ "Rename ship", application::UIState::GoTo(::UIState::IN_PLAY_RENAME_SHIP_ADJECTIVE) });
		MenuAction defaultAction = { "Never mind", application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA_OVERVIEW) };
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
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoMenuInput(
				Terminal::INVALID_INPUT,
				Refresh));
	}
}