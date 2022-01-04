#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.Utility.List.h>
#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.HeadForKnown.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_HEAD_FOR_KNOWN;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Head for:");
		auto character = game::Session().GetPlayer().GetCharacter();
		auto known = character.GetKnownIslands();
		auto location = character.GetBerth().GetShip().GetLocation();
		if (known.HasAny())
		{
			Terminal::SetForeground(game::Colors::GRAY);
			Terminal::WriteLine("Known islands:");
			Terminal::SetForeground(game::Colors::YELLOW);
			int index = 1;
			for (auto& island : known.GetAll())
			{
				auto relativeLocation = island.GetIsland().GetLocation() - location;
				Terminal::WriteLine("{}) {} ({:.2f}\xf8 dist {:.1f})",
					index++,
					island.GetDisplayName(),
					common::Heading::XYToDegrees(relativeLocation),
					relativeLocation.GetMagnitude());
			}
		}

		Terminal::WriteLine("0) Never mind");

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
			application::UIState::Write(::UIState::IN_PLAY_AT_SEA_OVERVIEW);
		};
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		auto character = game::Session().GetPlayer().GetCharacter();
		auto known = character.GetKnownIslands();
		auto location = character.GetBerth().GetShip().GetLocation();
		for (auto& island : known.GetAll())
		{
			auto relativeLocation = island.GetIsland().GetLocation() - location;
			Terminal::menu.AddAction(
				{ 
					std::format(
						"{} ({:.2f}\xf8 dist {:.1f})",
						island.GetDisplayName(),
						common::Heading::XYToDegrees(relativeLocation),
						relativeLocation.GetMagnitude()), 
					DoSetHeading(island, common::Heading::XYToDegrees(relativeLocation))
				});
		}
		MenuAction defaultAction = { "Never mind", application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA_OVERVIEW) };
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
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoMenuInput(
				Terminal::INVALID_INPUT,
				Refresh));
	}
}