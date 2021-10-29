#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include <Game.Islands.h>
#include <Game.Ship.h>
#include "State.InPlay.ChangeHeading.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CHANGE_HEADING;
	static const std::string LAYOUT_NAME = "State.Terminal";

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Change Heading:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Current Headomg: {:.2f}", game::Ship::GetHeading());

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Head for a known island");
		Terminal::WriteLine("2) Head for a nearby island");
		Terminal::WriteLine("3) Set heading manually");
		Terminal::WriteLine("4) Never mind");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static void OnHeadForKnownIsland()
	{
		Terminal::WriteLine();
		Terminal::SetForeground(game::Colors::RED);
		Terminal::WriteLine("TODO: head for known island");
		Refresh();
	}

	static void OnHeadForNearbyIsland()
	{
		if (!game::Islands::GetViewableIslands().empty())
		{
			application::UIState::Write(::UIState::IN_PLAY_HEAD_FOR_NEAR_BY);
		}
		else
		{
			Terminal::ErrorMessage("Please make a valid selection.");
		}
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", OnHeadForKnownIsland},
		{"2", OnHeadForNearbyIsland},
		{"3", application::UIState::GoTo(::UIState::IN_PLAY_MANUAL_HEADING)},
		{"4", application::UIState::GoTo(::UIState::IN_PLAY_SHIP_STATUS)}
	};

	void ChangeHeading::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				"Please enter a number between 1 and 4.",
				Refresh));
	}
}