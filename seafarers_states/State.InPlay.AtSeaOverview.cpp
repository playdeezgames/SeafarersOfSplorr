#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Heading.h>
#include <Game.h>
#include <Game.Audio.Mux.h>
#include <Game.Character.Actions.h>
#include <Game.Character.Docked.h>
#include <Game.Character.Items.h>
#include <Game.Character.Quest.h>
#include <Game.Character.Ship.h>
#include <Game.Avatar.Statistics.h>
#include <Game.Colors.h>
#include <Game.Fisheries.h>
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

	static int GetAvatarShipId()
	{
		return game::character::Ship::ReadShipId(game::Player::GetAvatarId()).value();
	}

	static bool IsFishingEnabled()
	{
		return
			game::character::Items::Has(game::Player::GetAvatarId(), game::Item::FISHING_POLE) &&
			game::character::Items::Has(game::Player::GetAvatarId(), game::Item::BAIT);
	}

	static bool RefreshDockableIslands()
	{
		auto dockable = game::Islands::GetDockableIslands();
		if (!dockable.empty())
		{
			auto island = dockable.front();
			if (island.visits.has_value())
			{
				Terminal::Write("You can dock at {}", island.GetDisplayName());
			}
			else
			{
				Terminal::Write("You can dock");
			}
			Terminal::WriteLine();
			return true;
		}
		return false;
	}

	static void RefreshNearbyIslands()
	{
		auto nearby = game::Islands::GetViewableIslands();
		if (!nearby.empty())
		{
			Terminal::Write("You see {} islands nearby", nearby.size());
			bool first = true;
			for (auto& island : nearby)
			{
				if (island.IsKnown())
				{
					if (first)
					{
						Terminal::Write(" including ");
						first = false;
					}
					else
					{
						Terminal::Write(", ");
					}
					Terminal::Write(island.GetDisplayName());
				}
			}
			Terminal::WriteLine();

		}
		else
		{
			Terminal::WriteLine("No islands are visible.");
		}
	}

	static bool RefreshJobDestination()
	{
		auto quest = game::character::Quest::Read();
		if (quest)
		{
			auto delta = quest.value().destination - game::Ship::GetLocation(GetAvatarShipId()).value();
			Terminal::WriteLine(
				"Delivery distance: {:.1f}", 
				delta.GetMagnitude());			
			return true;
		}
		return false;
	}

	static void RefreshFisheries()
	{
		if (!game::Fisheries::Available().empty())
		{
			Terminal::WriteLine("This looks like a good place to fish.");
		}
	}

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
			"Heading: {:.2f}\xf8, Speed: {:.1f}",
			game::Ship::GetHeading(GetAvatarShipId()).value(),
			game::Ship::GetSpeed(GetAvatarShipId()).value());
		Terminal::WriteLine(
			"Wind: {:.2f}\xf8 (x{:.1f})",
			game::World::GetWindHeading(),
			game::World::GetWindSpeedMultiplier(game::Ship::GetHeading(GetAvatarShipId()).value()));
		RefreshFisheries();
		bool canDock = RefreshDockableIslands();
		RefreshNearbyIslands();
		bool hasJob = RefreshJobDestination();
		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Move");
		Terminal::WriteLine("2) Multiple move");
		if (canDock)
		{
			Terminal::WriteLine("3) Dock/careen");
		}
		Terminal::WriteLine("4) Crew status");
		Terminal::WriteLine("5) Ship status");
		if (hasJob)
		{
			Terminal::WriteLine("6) Job status");
		}
		if (IsFishingEnabled())
		{
			Terminal::WriteLine("7) Fish");
		}
		Terminal::WriteLine("0) Menu");

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
		game::ApplyTurnEffects();
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
			Terminal::ErrorMessage(Terminal::INVALID_INPUT);
		}
		Refresh();
	}

	static void OnFishing()
	{
		if (IsFishingEnabled())
		{
			game::character::Actions::DoAction(game::Player::GetAvatarId(), game::character::Action::START_FISHING);
			application::UIState::Write(::UIState::IN_PLAY_NEXT);
		}
		else
		{
			Terminal::ErrorMessage(Terminal::INVALID_INPUT);
			Refresh();
		}
	}

	static void OnJob()
	{
		if (game::character::Quest::Read().has_value())
		{
			application::UIState::Write(::UIState::IN_PLAY_CURRENT_JOB);
		}
		else
		{
			Terminal::ErrorMessage(Terminal::INVALID_INPUT);
			Refresh();
		}
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", OnMove },
		{"2", application::UIState::GoTo(::UIState::IN_PLAY_MULTIPLE_MOVE) },
		{"3", OnDock },
		{"4", application::UIState::GoTo(::UIState::IN_PLAY_CREW_LIST) },
		{"5", application::UIState::GoTo(::UIState::IN_PLAY_SHIP_STATUS) },
		{"6", OnJob },
		{"7", OnFishing},
		{"0", application::UIState::GoTo(::UIState::LEAVE_PLAY) }

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