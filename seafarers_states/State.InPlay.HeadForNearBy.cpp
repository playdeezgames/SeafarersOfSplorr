#include "State.InPlay.HeadForNearBy.h"
#include "State.InPlay.AtSeaOverview.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.ShipStatus.h"
#include <algorithm>
#include <Common.Heading.h>
#include <Game.Session.Player.h>
#include <Game.Session.Character.h>
#include <Game.Session.Character.Berth.h>
#include <Game.Session.Character.KnownIslands.h>
#include <Game.Session.Ship.VisibleIslands.h>
#include <Game.Session.Island.h>
#include <Game.Session.Ship.h>
namespace state::in_play
{
	std::optional<int> HeadForNearBy::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Head for:");
		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}


	static std::function<void()> DoSetHeading(double heading)
	{
		return[heading]()
		{
			game::session::Ship(
			game::session::character::Berth(
				game::session::Player::GetCharacterId()).GetShipId())
				.SetHeading(heading);
			application::UIState::Write(ShipStatus::GetStateId());
		};
	}


	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		auto character = game::session::Character(game::session::Player::GetCharacterId());
		auto islands = 
			game::session::ship::VisibleIslands(
				game::session::character::Berth(game::session::Player::GetCharacterId()).GetShipId()).GetIslandIds();
		auto location = 
			game::session::Ship(
			game::session::character::Berth(game::session::Player::GetCharacterId()).GetShipId())
			.GetLocation();
		std::for_each(
			islands.begin(), 
			islands.end(), 
			[location, character](const auto& islandId) 
			{
				auto island = game::session::Island(islandId);
				auto relativeLocation = island.GetLocation() - location;
				auto knownIsland = game::session::character::KnownIslands(character.ToId()).GetKnownIsland(island);
				Terminal::menu.AddAction(
					{
						std::format(
							"{} ({:.2f}\xf8 dist {:.1f})",
							knownIsland.GetDisplayName(),
							common::Heading::XYToDegrees(relativeLocation),
							relativeLocation.GetMagnitude()),
						DoSetHeading(common::Heading::XYToDegrees(relativeLocation))
					});
			});
		MenuAction defaultAction = { "Never mind", application::UIState::DoGoTo(AtSeaOverview::GetStateId) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}
	void HeadForNearBy::Start()
	{
		Registrar::Register(stateId, [](int currentState)
			{
				::application::OnEnter::AddHandler(currentState, OnEnter);
				::application::Renderer::SetRenderLayout(currentState, Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					currentState,
					Terminal::DoMenuInput(
						Terminal::INVALID_INPUT,
						Refresh));
			});
	}
}