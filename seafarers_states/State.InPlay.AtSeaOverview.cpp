#include "State.InPlay.AtSeaOverview.h"
#include "State.InPlay.CrewList.h"
#include "State.InPlay.DockOrCareen.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.MultipleMove.h"
#include "State.InPlay.Next.h"
#include "State.InPlay.ShipStatus.h"
#include "State.LeavePlay.h"
#include <Game.Session.h>
#include <Game.Session.Player.h>
#include <Game.Session.World.Calendar.h>
#include <Game.Session.World.Wind.h>
namespace state::in_play
{
	std::optional<int> AtSeaOverview::stateId = std::nullopt;

	static bool RefreshDockableIslands()
	{
		auto character =
			game::session::Player()
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
			game::session::Player().GetCharacter();
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
			game::session::Player().GetCharacter();

		Terminal::Reinitialize();

		Terminal::SetForeground(
			game::Colors::LIGHT_CYAN);
		Terminal::WriteLine(
			"At Sea:");
		Terminal::SetForeground(
			game::Colors::GRAY);
		Terminal::WriteLine(game::session::world::Calendar().GetDate());

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
		auto wind = game::session::world::Wind();
		Terminal::WriteLine(
			"Wind: {:.2f}\xf8 (x{:.1f})",
			wind.GetHeading(),
			wind.GetSpeedMultiplier(ship.GetHeading()));
		//dockable islands
		RefreshDockableIslands();
		//nearby islands
		RefreshNearbyIslands();

		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static void OnMove()
	{
		game::Session::ApplyTurnEffects();
		application::UIState::Write(Next::GetStateId());
	}

	static void OnDock()
	{
		if (game::session::Player().GetCharacter()
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
		if (game::session::Player().GetCharacter().GetBerth().GetShip().CanDock())
		{
			Terminal::menu.AddAction({ "Dock", OnDock });
		}
		Terminal::menu.AddAction({ "Crew Status", application::UIState::DoGoTo(CrewList::GetStateId) });
		Terminal::menu.AddAction({ "Ship Status", application::UIState::DoGoTo(ShipStatus::GetStateId) });
		MenuAction defaultAction = { "Menu", application::UIState::DoGoTo(LeavePlay::GetStateId) };
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
		Registrar::Register(stateId, [](int currentState)
			{
				::application::OnEnter::AddHandler(
					currentState,
					OnEnter);
				::application::Renderer::SetRenderLayout(
					currentState,
					Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					currentState,
					Terminal::DoMenuInput(
						Terminal::INVALID_INPUT,
						Refresh));
			});
	}
}