#include <Common.Heading.h>
#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.AtSeaOverview.h"
#include "Menu.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_AT_SEA_OVERVIEW;

	static bool RefreshDockableIslands()
	{
		auto character =
			game::Session()
			.GetPlayer()
			.GetCharacter();
		auto island =
			character
			.GetBerth()
			.GetShip()
			.GetDockableIslands()
			.TryGetFirst();
		if (island)
		{
			auto knownIsland =
				character
				.GetKnownIslands()
				.GetKnownIsland(island.value());
			Terminal::SetForeground(game::Colors::LIGHT_BLUE);
			if (knownIsland.IsKnown())
			{
				Terminal::Write("You can dock at {}", knownIsland.GetDisplayName());
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
		auto character =
			game::Session()
			.GetPlayer().GetCharacter();
		auto islands =
			character
			.GetBerth()
			.GetShip()
			.GetNearbyIslands();
		if (islands.HasAny())
		{
			Terminal::Write("You see {} islands nearby", islands.GetCount());
			bool first = true;
			for (auto& island : islands.GetAll())
			{
				auto knownIsland =
					character
					.GetKnownIslands()
					.GetKnownIsland(island);
				if (knownIsland.IsKnown())
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
					Terminal::Write(knownIsland.GetDisplayName());
				}
			}
			Terminal::WriteLine();
		}
		else
		{
			Terminal::WriteLine("No islands are visible.");
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
		Terminal::WriteLine(game::Session().GetWorld().GetCalendar().GetDate());

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
		//dockable islands
		bool canDock = RefreshDockableIslands();
		//nearby islands
		RefreshNearbyIslands();

		Terminal::ShowMenu();

		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine();
		Terminal::Write(">");
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
			.TryGetFirst())
		{
			application::UIState::Write(::UIState::IN_PLAY_DOCK_OR_CAREEN);
		}
		else
		{
			Terminal::ErrorMessage(Terminal::INVALID_INPUT);
		}
		Refresh();
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.AddAction({ "Move", OnMove });
		Terminal::menu.AddAction({ "Multiple move", application::UIState::GoTo(::UIState::IN_PLAY_MULTIPLE_MOVE) });
		if (game::Session().GetPlayer().GetCharacter().GetBerth().GetShip().CanDock())
		{
			Terminal::menu.AddAction({ "Dock", OnDock });
		}
		Terminal::menu.AddAction({ "Crew Status", application::UIState::GoTo(::UIState::IN_PLAY_CREW_LIST) });
		Terminal::menu.AddAction({ "Ship Status", application::UIState::GoTo(::UIState::IN_PLAY_SHIP_STATUS) });
		MenuAction defaultAction = { "Menu", application::UIState::GoTo(::UIState::LEAVE_PLAY) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", OnMove },
		{"2", application::UIState::GoTo(::UIState::IN_PLAY_MULTIPLE_MOVE) },
		{"3", OnDock },
		{"4", application::UIState::GoTo(::UIState::IN_PLAY_CREW_LIST) },
		{"5", application::UIState::GoTo(::UIState::IN_PLAY_SHIP_STATUS) },
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
			Terminal::DoMenuInput(
				Terminal::INVALID_INPUT, 
				Refresh));
	}
}