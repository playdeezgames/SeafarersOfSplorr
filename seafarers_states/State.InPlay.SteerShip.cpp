#include <Application.Keyboard.h>
#include <Application.Renderer.h>
#include <Application.OnEnter.h>
#include <Application.UIState.h>
#include <functional>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include <Game.Ship.h>
#include <map>
#include <string>
#include "State.InPlay.SteerShip.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_STEER_SHIP;
	static const std::string LAYOUT_NAME = "State.Terminal";

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"3", application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA_OVERVIEW)}
	};

	static void Refresh()
	{
		Terminal::ClearStatusLine();
		Terminal::ClearInput();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine();
		Terminal::WriteLine("Change heading or speed:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Current Heading: {:.2f}", game::Ship::GetHeading());
		Terminal::WriteLine("Current Speed: {:.2f}", game::Ship::GetSpeed());

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Change Heading");
		Terminal::WriteLine("2) Change Speed");
		Terminal::WriteLine("3) Never mind");

		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine();
		Terminal::Write(">");
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	void SteerShip::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
		::application::Keyboard::AddHandler(CURRENT_STATE, Terminal::DoIntegerInput(menuActions, "Please enter a number between 1 and 3.", Refresh));
	}
}