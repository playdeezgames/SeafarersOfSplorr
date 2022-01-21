#include "State.InPlay.HeadForNearBy.h"
#include "State.InPlay.AtSeaOverview.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.ShipStatus.h"
#include <algorithm>
#include <Common.Heading.h>
#include <Game.Session.Player.h>
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


	static std::function<void()> DoSetHeading(const game::session::Island& island, double heading)
	{
		return[island, heading]()
		{
			game::session::Player().GetCharacter().GetBerth().GetShip().SetHeading(heading);
			application::UIState::Write(ShipStatus::GetStateId());
		};
	}


	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		auto character = game::session::Player().GetCharacter();
		auto islands = character.GetBerth().GetShip().GetNearbyIslands().GetAll();
		auto location = character.GetBerth().GetShip().GetLocation();
		std::for_each(
			islands.begin(), 
			islands.end(), 
			[location, character](const game::session::Island& island) 
			{
				auto relativeLocation = island.GetLocation() - location;
				auto knownIsland = character.GetKnownIslands().GetKnownIsland(island);
				Terminal::menu.AddAction(
					{
						std::format(
							"{} ({:.2f}\xf8 dist {:.1f})",
							knownIsland.GetDisplayName(),
							common::Heading::XYToDegrees(relativeLocation),
							relativeLocation.GetMagnitude()),
						DoSetHeading(island, common::Heading::XYToDegrees(relativeLocation))
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