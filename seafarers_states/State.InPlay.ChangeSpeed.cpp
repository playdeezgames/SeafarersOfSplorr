#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include <Game.Character.Ship.h>
#include <Game.Colors.h>
#include <Game.Player.h>
#include <Game.Ship.h>
#include "State.InPlay.ChangeSpeed.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CHANGE_SPEED;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Change Speed:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Current Speed: {:.2f}", game::Ship::GetSpeed(game::character::Ship::ReadShipId(game::Player::GetAvatarId()).value()).value());

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) All stop");
		Terminal::WriteLine("2) Ahead 1/3");
		Terminal::WriteLine("3) Ahead 2/3");
		Terminal::WriteLine("4) Ahead full");
		Terminal::WriteLine("5) Ahead flank");
		Terminal::WriteLine("6) Never mind");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static std::function<void()> DoSetSpeed(double speed)
	{
		return [speed]() 
		{
			game::Ship::SetSpeed(game::character::Ship::ReadShipId(game::Player::GetAvatarId()).value(), speed);
			application::UIState::Write(::UIState::IN_PLAY_SHIP_STATUS);
		};
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", DoSetSpeed(0.0)},
		{"2", DoSetSpeed(0.3)},
		{"3", DoSetSpeed(0.6)},
		{"4", DoSetSpeed(0.9)},
		{"5", DoSetSpeed(1.0)},
		{"6", application::UIState::GoTo(::UIState::IN_PLAY_SHIP_STATUS)}
	};

	void ChangeSpeed::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE, 
			Terminal::DoIntegerInput(
				menuActions, 
				"Please enter a number between 1 and 6.", 
				Refresh));
	}
}