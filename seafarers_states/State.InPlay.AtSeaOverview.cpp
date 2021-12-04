#include <Common.Heading.h>
#include <Game.h>
#include <Game.Characters.Items.h>
#include <Game.Characters.Statistics.h>
#include <Game.Fisheries.h>
#include <Game.Islands.h>
#include <Game.Player.h>
#include <Game.Ship.h>
#include <Game.World.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.AtSeaOverview.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_AT_SEA_OVERVIEW;

	static bool IsFishingEnabled()
	{
		return
			game::character::Items::Has(GetPlayerCharacterId(), game::Item::FISHING_POLE) &&
			game::character::Items::Has(GetPlayerCharacterId(), game::Item::BAIT);
	}

	static bool RefreshDockableIslands()
	{
		auto dockable = game::Islands::GetDockableIslands(GetPlayerCharacterShipId().value());
		if (!dockable.empty())
		{
			auto island = dockable.front();
			Terminal::SetForeground(game::Colors::LIGHT_BLUE);
			if (island.visits.has_value())
			{
				Terminal::Write("You can dock at {}", island.GetDisplayName());
			}
			else
			{
				Terminal::Write("You can dock");
			}
			Terminal::SetForeground(game::Colors::GRAY);
			Terminal::WriteLine();
			return true;
		}
		return false;
	}

	static void RefreshNearbyIslands()
	{
		auto nearby = GetPlayerCharacterViewableIslands();
		if (nearby && !nearby.value().empty())
		{
			Terminal::Write("You see {} islands nearby", nearby.value().size());
			bool first = true;
			for (auto& island : nearby.value())
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
		auto quest = GetPlayerCharacterQuest();
		if (quest)
		{
			auto delta = quest.value().destination - GetPlayerCharacterShipLocation().value();
			Terminal::WriteLine(
				"Delivery distance: {:.1f}", 
				delta.GetMagnitude());			
			return true;
		}
		return false;
	}

	static void RefreshFisheries()
	{
		if (!game::Fisheries::Available(GetPlayerCharacterShipId().value()).empty())
		{
			Terminal::WriteLine("This looks like a good place to fish.");
		}
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(
			game::Colors::LIGHT_CYAN);
		Terminal::WriteLine(
			"At Sea:");
		Terminal::SetForeground(
			game::Colors::GRAY);
		Terminal::WriteLine(
			"Heading: {:.2f}\xf8, Speed: {:.1f}",
			GetPlayerCharacterShipHeading().value(),
			GetPlayerCharacterShipSpeed().value());
		Terminal::WriteLine(
			"Wind: {:.2f}\xf8 (x{:.1f})",
			game::World::GetWindHeading(),
			game::World::GetWindSpeedMultiplier(GetPlayerCharacterShipHeading().value()));
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
		PlayMainTheme();
		Refresh();
	}

	static void OnMove()
	{
		AddPlayerCharacterMessage(game::Colors::GREEN, "Steady as she goes!");
		game::ApplyTurnEffects();
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnDock()
	{
		if (!game::Islands::GetDockableIslands(GetPlayerCharacterShipId().value()).empty())
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
			DoPlayerCharacterAction(game::character::Action::START_FISHING);
			application::UIState::Write(::UIState::IN_PLAY_NEXT);
			return;
		}
		Terminal::ErrorMessage(Terminal::INVALID_INPUT);
		Refresh();
	}

	static void OnJob()
	{
		if (GetPlayerCharacterQuest())
		{
			application::UIState::Write(::UIState::IN_PLAY_CURRENT_JOB);
			return;
		}
		Terminal::ErrorMessage(Terminal::INVALID_INPUT);
		Refresh();
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