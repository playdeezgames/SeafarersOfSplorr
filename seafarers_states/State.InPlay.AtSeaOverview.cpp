#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include <Game.Ship.h>
#include "State.Terminal.h"
#include "States.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_AT_SEA_OVERVIEW;
	static const std::string LAYOUT_NAME = "State.Terminal";

	static void Refresh()
	{
		Terminal::ClearStatusLine();
		Terminal::ClearInput();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine();
		Terminal::WriteLine("At Sea:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Heading: {:.2f}, Speed: {:.2f}", game::Ship::GetHeading(), game::Ship::GetSpeed());

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Move");
		Terminal::WriteLine("2) Change Heading");
		Terminal::WriteLine("3) Change Speed");

		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine();
		Terminal::Write(">");
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"0", application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA_DEPRECATED)}
	};

	void AtSeaOverview::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
		::application::Keyboard::AddHandler(CURRENT_STATE, Terminal::DoIntegerInput(menuActions, "Please enter a number between 1 and X", Refresh));
	}
}