#include <Application.Keyboard.h>
#include <Application.Renderer.h>
#include <Application.OnEnter.h>
#include <Application.UIState.h>
#include <functional>
#include <Game.Audio.Mux.h>
#include <Game.Character.Ship.h>
#include <Game.Colors.h>
#include <Game.Player.h>
#include <Game.Ship.h>
#include <map>
#include <string>
#include "State.InPlay.ShipStatus.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_SHIP_STATUS;

	static int GetAvatarShipId()
	{
		return game::character::Ship::ReadShipId(game::Player::GetCharacterId()).value();
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Ship status:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Name: {}", game::Ship::GetName(GetAvatarShipId()).value());

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Change Heading(Current: {:.2f}\xf8)", game::Ship::GetHeading(GetAvatarShipId()).value());
		Terminal::WriteLine("2) Change Speed(Current: {:.1f})", game::Ship::GetSpeed(GetAvatarShipId()).value());
		Terminal::WriteLine("3) Cargo");
		Terminal::WriteLine("4) Rename ship");
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", application::UIState::GoTo(::UIState::IN_PLAY_CHANGE_HEADING)},
		{"2", application::UIState::GoTo(::UIState::IN_PLAY_CHANGE_SPEED)},
		{"3", application::UIState::GoTo(::UIState::IN_PLAY_CARGO)},
		{"4", application::UIState::GoTo(::UIState::IN_PLAY_RENAME_SHIP_ADJECTIVE)},
		{"0", application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA_OVERVIEW)}
	};

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	void ShipStatus::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE, 
			Terminal::DoIntegerInput(
				menuActions, 
				"Please enter a number between 1 and 4.", 
				Refresh));
	}
}