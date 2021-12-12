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
		auto items = GetGameSession().GetPlayerCharacter().value().GetItems();
		return
			items.Has(game::Item::FISHING_POLE) &&
			items.Has(game::Item::BAIT);
	}

	static bool RefreshDockableIslands()
	{
		auto island =
			GetGameSession()
			.GetPlayerCharacter().value()
			.GetBerth().value()
			.GetShip().value()
			.GetDockableIslands()
			.GetFirst();
		if (island)
		{
			Terminal::SetForeground(game::Colors::LIGHT_BLUE);
			if (island.value().IsKnown().value())
			{
				Terminal::Write("You can dock at {}", island.value().GetDisplayName().value());
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
		auto islands =
			GetGameSession()
			.GetPlayerCharacter().value()
			.GetBerth().value()
			.GetShip().value()
			.GetNearbyIslands();
		if (islands.HasAny())
		{
			Terminal::Write("You see {} islands nearby", islands.GetCount());
			bool first = true;
			for (auto& island : islands.GetAll())
			{
				if (island.IsKnown().value())
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
					Terminal::Write(island.GetDisplayName().value());
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
		auto quest =
			GetGameSession()
			.GetPlayerCharacter().value()
			.GetQuest();
		if (quest)
		{
			auto destination = 
				quest.value().
				GetDestinationIsland().value().
				GetLocation().value();
			auto location = 
				GetGameSession()
				.GetPlayerCharacter().value()
				.GetBerth().value()
				.GetShip().value()
				.GetLocation().value();
			auto delta = destination - location;
			Terminal::WriteLine(
				"Delivery distance: {:.1f}", 
				delta.GetMagnitude());			
			return true;
		}
		return false;
	}

	static void RefreshFisheries()
	{
		if(GetGameSession()
			.GetPlayerCharacter().value()
			.GetBerth().value()
			.GetShip().value()
			.GetFisheries()
			.HasAny())
		{
			Terminal::WriteLine("This looks like a good place to fish.");
		}
	}

	static void Refresh()
	{
		auto playerCharacter = 
			GetGameSession()
			.GetPlayerCharacter()
			.value();

		Terminal::Reinitialize();

		Terminal::SetForeground(
			game::Colors::LIGHT_CYAN);
		Terminal::WriteLine(
			"At Sea:");
		Terminal::SetForeground(
			game::Colors::GRAY);
		//ship
		auto ship = 
			playerCharacter
			.GetBerth().value()
			.GetShip().value();
		Terminal::WriteLine(
			"Heading: {:.2f}\xf8, Speed: {:.1f}",
			ship.GetHeading().value(),
			ship.GetSpeed().value());
		//wind
		auto wind = GetGameSession().GetWorld().GetWind();
		Terminal::WriteLine(
			"Wind: {:.2f}\xf8 (x{:.1f})",
			wind.GetHeading(),
			wind.GetMultiplier(ship.GetHeading().value()));
		//fisheries
		RefreshFisheries();
		//dockable islands
		bool canDock = RefreshDockableIslands();
		//nearby islands
		RefreshNearbyIslands();
		//job destination
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
		GetGameSession()
			.GetPlayerCharacter().value()
			.GetMessages()
			.Add(game::Colors::GREEN, "Steady as she goes!");
		GetGameSession().ApplyTurnEffects();
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnDock()
	{
		if (GetGameSession()
			.GetPlayerCharacter().value()
			.GetBerth().value()
			.GetShip().value()
			.GetDockableIslands()
			.HasAny())
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
			GetGameSession()
			.GetPlayerCharacter().value()
			.DoAction(game::characters::Action::START_FISHING);
			application::UIState::Write(::UIState::IN_PLAY_NEXT);
			return;
		}
		Terminal::ErrorMessage(Terminal::INVALID_INPUT);
		Refresh();
	}

	static void OnJob()
	{
		if (GetGameSession()
			.GetPlayerCharacter().value()
			.GetQuest())
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