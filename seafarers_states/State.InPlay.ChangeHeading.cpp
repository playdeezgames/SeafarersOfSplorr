#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Heading.h>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.Quest.h>
#include <Game.Avatar.Ship.h>
#include <Game.Ship.h>
#include <Game.Colors.h>
#include <Game.Islands.h>
#include <Game.Ship.h>
#include "State.InPlay.ChangeHeading.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CHANGE_HEADING;

	static int GetAvatarShipId()
	{
		return game::avatar::Ship::Read().value().shipId;
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Change Heading:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Current: {:.2f}\xf8", game::Ship::GetHeading(GetAvatarShipId()).value());

		Terminal::SetForeground(game::Colors::YELLOW);
		if (!game::Islands::GetKnownIslands(game::Ship::GetLocation(game::avatar::Ship::ReadShipId().value()).value()).empty())
		{
			Terminal::WriteLine("1) Head for a known island");
		}
		if (!game::Islands::GetViewableIslands().empty())
		{
			Terminal::WriteLine("2) Head for a nearby island");
		}
		if (game::avatar::Quest::Read())
		{
			Terminal::WriteLine("3) Head for job destination");
		}
		Terminal::WriteLine("4) Set heading manually");
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static void OnHeadForKnownIsland()
	{
		if (!game::Islands::GetKnownIslands(game::Ship::GetLocation(game::avatar::Ship::ReadShipId().value()).value()).empty())
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
		if (!game::Islands::GetViewableIslands().empty())
		{
			application::UIState::Write(::UIState::IN_PLAY_HEAD_FOR_NEAR_BY);
		}
		else
		{
			Terminal::ErrorMessage(Terminal::INVALID_INPUT);
			Refresh();
		}
	}

	static void OnHeadForJobDestination()
	{
		auto quest = game::avatar::Quest::Read();
		if (quest)
		{
			auto delta = quest.value().destination - game::Ship::GetLocation(game::avatar::Ship::ReadShipId().value()).value();
			auto island = game::Islands::Read(quest.value().destination);
			game::Ship::SetHeading(common::Heading::XYToDegrees(delta));
			Terminal::SetForeground(game::Colors::GREEN);
			Terminal::WriteLine();
			Terminal::WriteLine("You head for {}.", island.value().name);
			application::UIState::Write(::UIState::IN_PLAY_NEXT);
		}
		else
		{
			Terminal::ErrorMessage("Please select a valid option.");
			Refresh();
		}
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", OnHeadForKnownIsland},
		{"2", OnHeadForNearbyIsland},
		{"3", OnHeadForJobDestination},
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