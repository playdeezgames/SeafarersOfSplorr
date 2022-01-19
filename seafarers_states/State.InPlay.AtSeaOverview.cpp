#include <Common.Heading.h>
#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.AtSeaOverview.h"
#include "State.InPlay.DockOrCareen.h"
#include "State.InPlay.MultipleMove.h"
#include "State.Registrar.h"
#include "Menu.h"
namespace state::in_play
{
	std::optional<int> AtSeaOverview::stateId = std::nullopt;

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
			wind.GetSpeedMultiplier(ship.GetHeading()));
		//dockable islands
		bool canDock = RefreshDockableIslands();
		//nearby islands
		RefreshNearbyIslands();

		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static void OnMove()
	{
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
			application::UIState::Write(DockOrCareen::GetStateId());
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
		Terminal::menu.SetRefresh(Refresh);
		Terminal::menu.AddAction({ "Move", OnMove });
		Terminal::menu.AddAction({ "Multiple move", application::UIState::DoGoTo(MultipleMove::GetStateId) });
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

	void AtSeaOverview::Start()
	{
		Registrar::Register(stateId, [](int stateId)
			{
				::application::OnEnter::AddHandler(
					stateId,
					OnEnter);
				::application::Renderer::SetRenderLayout(
					stateId,
					Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					stateId,
					Terminal::DoMenuInput(
						Terminal::INVALID_INPUT,
						Refresh));
			});
	}
}