#include <algorithm>
#include <Common.Data.h>
#include <Common.Heading.h>
#include <Game.Session.h>
#include "State.InPlay.AtSeaOverview.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.HeadForKnown.h"
#include "State.InPlay.ShipStatus.h"
#include "State.Registrar.h"
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
			game::Session().GetPlayer().GetCharacter().GetBerth().GetShip().SetHeading(heading);
			Terminal::WriteLine("You head for {}.", island.GetDisplayName());
			application::UIState::Write(AtSeaOverview::GetStateId());
		};
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		auto character = game::Session().GetPlayer().GetCharacter();
		auto known = character.GetKnownIslands();
		auto location = game::Session().GetPlayer().GetCharacter().GetBerth().GetShip().GetLocation();
		auto islands = known.GetAll();
		std::for_each(
			islands.begin(), 
			islands.end(), 
			[character, location](const game::session::character::KnownIsland& knownIsland)
			{
				auto relativeLocation = knownIsland.GetIsland().GetLocation() - location;
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
		Registrar::Register(stateId, [](int stateId) 
			{
				::application::OnEnter::AddHandler(stateId, OnEnter);
				::application::Renderer::SetRenderLayout(stateId, Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					stateId,
					Terminal::DoMenuInput(
						Terminal::INVALID_INPUT,
						Refresh));
			});
	}
}