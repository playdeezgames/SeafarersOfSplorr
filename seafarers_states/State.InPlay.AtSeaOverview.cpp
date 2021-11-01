#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Heading.h>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.Docked.h>
#include <Game.Avatar.Quest.h>
#include <Game.Avatar.Statistics.h>
#include <Game.Colors.h>
#include <Game.Islands.h>
#include <Game.Player.h>
#include <Game.Ship.h>
#include <Game.World.h>
#include "State.Terminal.h"
#include "State.InPlay.AtSeaOverview.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_AT_SEA_OVERVIEW;

	static void Refresh()
	{
		Terminal::ClearStatusLine();
		Terminal::ClearInput();

		Terminal::SetForeground(
			game::Colors::LIGHT_CYAN);
		Terminal::WriteLine();
		Terminal::WriteLine(
			"At Sea:");
		Terminal::SetForeground(
			game::Colors::GRAY);
		Terminal::WriteLine(
			"Turns remaining: {}", 
			game::avatar::Statistics::GetTurnsRemaining(game::Player::GetAvatarId()));
		Terminal::WriteLine(
			"Heading: {:.2f}\xf8, Speed: {:.1f}", 
			game::Ship::GetHeading(), 
			game::Ship::GetSpeed());
		Terminal::WriteLine(
			"Wind: {:.2f}\xf8 (x{:.1f})", 
			game::World::GetWindHeading(), 
			game::World::GetWindSpeedMultiplier(game::Ship::GetHeading()));
		auto nearby = game::Islands::GetViewableIslands();
		if (!nearby.empty())
		{
			Terminal::WriteLine("Nearby islands:");
			for (auto& island : nearby)
			{
				Terminal::WriteLine(
					"{} ({:.2f}\xf8 dist {:.1f})", 
					island.GetDisplayName(), 
					common::Heading::XYToDegrees(island.relativeLocation), 
					island.relativeLocation.GetMagnitude());
			}
		}
		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Move");
		Terminal::WriteLine("2) Multiple move");
		if (!game::Islands::GetDockableIslands().empty())
		{
			Terminal::WriteLine("3) Dock/careen");
		}
		Terminal::WriteLine("4) Crew status");
		Terminal::WriteLine("5) Ship status");
		Terminal::WriteLine("6) Job status");
		Terminal::WriteLine("7) Menu");

		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine();
		Terminal::Write(">");
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static void OnMove()
	{
		Terminal::SetForeground(game::Colors::GREEN);
		Terminal::WriteLine();
		Terminal::WriteLine("Steady as she goes!");
		game::Ship::Move();
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnDock()
	{
		if (!game::Islands::GetDockableIslands().empty())
		{
			application::UIState::Write(::UIState::IN_PLAY_DOCK_OR_CAREEN);
		}
		else
		{
			Terminal::ErrorMessage("There is no place to dock or careen!");
		}
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", OnMove },
		{"2", application::UIState::GoTo(::UIState::IN_PLAY_MULTIPLE_MOVE) },
		{"3", OnDock },
		{"4", application::UIState::GoTo(::UIState::IN_PLAY_CREW_LIST) },
		{"5", application::UIState::GoTo(::UIState::IN_PLAY_SHIP_STATUS) },
		{"6", application::UIState::GoTo(::UIState::IN_PLAY_CURRENT_JOB) },
		{"7", application::UIState::GoTo(::UIState::LEAVE_PLAY) }

		//delete this later
		, {"0", application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA_DEPRECATED)}
	};

	void AtSeaOverview::Start()
	{
		::application::OnEnter::AddHandler(
			CURRENT_STATE, 
			OnEnter);
		::application::Renderer::SetRenderLayout(
			CURRENT_STATE, 
			Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE, 
			Terminal::DoIntegerInput(
				menuActions, 
				Terminal::INVALID_INPUT, 
				Refresh));
	}
}