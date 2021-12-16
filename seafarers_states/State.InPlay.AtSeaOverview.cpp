#include <Common.Heading.h>
#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.AtSeaOverview.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_AT_SEA_OVERVIEW;

	static bool IsFishingEnabled()
	{
		auto items = game::Session().GetPlayer().GetCharacter().GetLegacyItems();
		return
			items.Has(game::Item::FISHING_POLE) &&
			items.Has(game::Item::BAIT);
	}

	static bool RefreshDockableIslands()
	{
		auto island =
			game::Session()
			.GetPlayer().GetCharacter()
			.GetBerth()
			.GetShip()
			.GetDockableIslands()
			.TryGetFirst();
		if (island)
		{
			Terminal::SetForeground(game::Colors::LIGHT_BLUE);
			if (island.value().IsKnown())
			{
				Terminal::Write("You can dock at {}", island.value().GetDisplayName());
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
			game::Session()
			.GetPlayer().GetCharacter()
			.GetBerth()
			.GetShip()
			.GetNearbyIslands();
		if (islands.HasAny())
		{
			Terminal::Write("You see {} islands nearby", islands.GetCount());
			bool first = true;
			for (auto& island : islands.GetAll())
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
		auto quest =
			game::Session()
			.GetPlayer().GetCharacter()
			.TryGetQuest();
		if (quest)
		{
			auto destination = 
				quest.value()
				.GetDestinationIsland()
				.GetLocation();
			auto location = 
				game::Session()
				.GetPlayer().GetCharacter()
				.GetBerth()
				.GetShip()
				.GetLocation();
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
		if(game::Session()
			.GetPlayer().GetCharacter()
			.GetBerth()
			.GetShip()
			.GetFisheries()
			.HasAny())
		{
			Terminal::WriteLine("This looks like a good place to fish.");
		}
	}

	static void Refresh()
	{
		auto playerCharacter = 
			game::Session()
			.GetPlayer().GetCharacter();

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
			.GetBerth()
			.GetShip();
		Terminal::WriteLine(
			"Heading: {:.2f}\xf8, Speed: {:.1f}",
			ship.GetHeading(),
			ship.GetSpeed());
		//wind
		auto wind = game::Session().GetWorld().GetWind();
		Terminal::WriteLine(
			"Wind: {:.2f}\xf8 (x{:.1f})",
			wind.GetHeading(),
			wind.GetMultiplier(ship.GetHeading()));
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
		game::Session()
			.GetPlayer().GetCharacter()
			.GetMessages()
			.Add(game::Colors::GREEN, "Steady as she goes!");
		game::Session().ApplyTurnEffects();
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnDock()
	{
		if (game::Session()
			.GetPlayer().GetCharacter()
			.GetBerth()
			.GetShip()
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
			game::Session()
			.GetPlayer().GetCharacter()
			.DoAction(game::characters::Action::START_FISHING);
			application::UIState::Write(::UIState::IN_PLAY_NEXT);
			return;
		}
		Terminal::ErrorMessage(Terminal::INVALID_INPUT);
		Refresh();
	}

	static void OnJob()
	{
		if (game::Session()
			.GetPlayer().GetCharacter()
			.TryGetQuest())
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