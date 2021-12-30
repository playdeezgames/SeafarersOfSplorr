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

		Terminal::SetForeground(game::Colors::YELLOW);
		if (playerCharacter
			.GetKnownIslands().HasAny())
		{
			Terminal::WriteLine("1) Head for a known island");
		}
		if (playerCharacter
			.GetBerth()
			.GetShip()
			.GetNearbyIslands().HasAny())
		{
			Terminal::WriteLine("2) Head for a nearby island");
		}
		Terminal::WriteLine("4) Set heading manually");
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		PlayMainTheme();
		Refresh();
	}

	static void OnHeadForKnownIsland()
	{
		if (game::Session()
			.GetPlayer().GetCharacter()
			.GetKnownIslands().HasAny())
		{
			application::UIState::Write(::UIState::IN_PLAY_HEAD_FOR_KNOWN);
		}
		else
		{
			Terminal::ErrorMessage(Terminal::INVALID_INPUT);
			Refresh();
		}
	}

	static void OnHeadForNearbyIsland()
	{
		if (game::Session()
			.GetPlayer().GetCharacter()
			.GetBerth()
			.GetShip()
			.GetNearbyIslands().HasAny())
		{
			application::UIState::Write(::UIState::IN_PLAY_HEAD_FOR_NEAR_BY);
		}
		else
		{
			Terminal::ErrorMessage(Terminal::INVALID_INPUT);
			Refresh();
		}
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", OnHeadForKnownIsland},
		{"2", OnHeadForNearbyIsland},
		{"4", application::UIState::GoTo(::UIState::IN_PLAY_MANUAL_HEADING)},
		{"0", application::UIState::GoTo(::UIState::IN_PLAY_SHIP_STATUS)}
	};

	void ChangeHeading::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				Terminal::INVALID_INPUT,
				Refresh));
	}
}