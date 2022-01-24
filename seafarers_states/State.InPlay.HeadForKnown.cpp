#include "State.InPlay.HeadForKnown.h"
#include "State.InPlay.AtSeaOverview.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.ShipStatus.h"
#include <algorithm>
#include <Common.Heading.h>
#include <Game.Session.Player.h>
#include <Game.Session.Character.h>
#include <Game.Session.Character.Berth.h>
#include <Game.Session.Character.KnownIslands.h>
#include <Game.Session.Island.h>
#include <Game.Session.Ship.h>
namespace state::in_play
{
	std::optional<int> HeadForKnown::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();
		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Head for:");
		Terminal::ShowMenu();
		Terminal::ShowPrompt();
	}

	static std::function<void()> DoSetHeading(const game::session::character::KnownIsland& island, double heading)
	{
		return[island, heading]()
		{
			Terminal::SetForeground(game::Colors::GREEN);
			Terminal::WriteLine();
			game::session::Ship(
			game::session::character::Berth(game::session::Player::GetCharacterId()).GetShipId())
				.SetHeading(heading);
			Terminal::WriteLine("You head for {}.", island.GetDisplayName());
			application::UIState::Write(AtSeaOverview::GetStateId());
		};
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		auto character = game::session::Character(game::session::Player::GetCharacterId());
		auto known = game::session::character::KnownIslands(character.ToId());
		auto location = 
			game::session::Ship(
			game::session::character::Berth(game::session::Player::GetCharacterId()).GetShipId())
			.GetLocation();
		auto islands = known.GetAll();
		std::for_each(
			islands.begin(), 
			islands.end(), 
			[character, location](const game::session::character::KnownIsland& knownIsland)
			{
				auto relativeLocation = game::session::Island(knownIsland.GetIslandId()).GetLocation() - location;
				Terminal::menu.AddAction({ std::format("{} ({:.2f}\xf8 dist {:.1f})",
					knownIsland.GetDisplayName(),
					common::Heading::XYToDegrees(relativeLocation),
					relativeLocation.GetMagnitude()),
					DoSetHeading(knownIsland, common::Heading::XYToDegrees(relativeLocation)) });
			});
		MenuAction defaultAction = { "Never mind", application::UIState::DoGoTo(ShipStatus::GetStateId) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void HeadForKnown::Start()
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