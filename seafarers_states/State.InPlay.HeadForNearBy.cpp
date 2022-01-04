#include <Common.Heading.h>
#include <Common.Utility.List.h>
#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.HeadForNearBy.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_HEAD_FOR_NEAR_BY;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Head for:");
		auto character = game::Session().GetPlayer().GetCharacter();
		auto nearby = character.GetBerth().GetShip().GetNearbyIslands();
		if (nearby.HasAny())
		{
			Terminal::SetForeground(game::Colors::GRAY);
			Terminal::WriteLine("Nearby islands:");
			Terminal::SetForeground(game::Colors::YELLOW);
			int index = 1;

			auto location = game::Session().GetPlayer().GetCharacter().GetBerth().GetShip().GetLocation();
			for (auto& island : nearby.GetAll())
			{
				auto relativeLocation = island.GetLocation() - location;
				auto knownIsland = character.GetKnownIslands().GetKnownIsland(island);
				Terminal::WriteLine("{}) {} ({:.2f}\xf8 dist {:.1f})",
					index++,
					knownIsland.GetDisplayName(),
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

	static std::function<void()> DoHeadForNearByIndex(size_t index)
	{
		return [index]() 
		{
			auto character = game::Session().GetPlayer().GetCharacter();
			auto nearby = character.GetBerth().GetShip().GetNearbyIslands().GetAll();
			auto chosen = common::utility::List::GetNth(nearby, index);
			if (chosen)
			{
				Terminal::SetForeground(game::Colors::GREEN);
				Terminal::WriteLine();
				auto location = game::Session().GetPlayer().GetCharacter().GetBerth().GetShip().GetLocation();
				auto relativeLocation = chosen.value().GetLocation() - location;
				game::Session().GetPlayer().GetCharacter().GetBerth().GetShip().SetHeading(common::Heading::XYToDegrees(relativeLocation));
				auto knownIsland = character.GetKnownIslands().GetKnownIsland(chosen.value());
				Terminal::WriteLine("You head for {}.", knownIsland.GetDisplayName());
				application::UIState::Write(::UIState::IN_PLAY_AT_SEA_OVERVIEW);
			}
			else
			{
				Terminal::SetForeground(game::Colors::RED);
				Terminal::WriteLine();
				Terminal::WriteLine(Terminal::INVALID_INPUT);
				Refresh();
			}
		};
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", DoHeadForNearByIndex(0)},
		{"2", DoHeadForNearByIndex(1)},
		{"3", DoHeadForNearByIndex(2)},
		{"4", DoHeadForNearByIndex(3)},
		{"5", DoHeadForNearByIndex(4)},
		{"6", DoHeadForNearByIndex(5)},
		{"7", DoHeadForNearByIndex(6)},
		{"8", DoHeadForNearByIndex(7)},
		{"9", DoHeadForNearByIndex(8)},
		{"0", application::UIState::GoTo(::UIState::IN_PLAY_CHANGE_HEADING)}
	};

	void HeadForNearBy::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				Terminal::INVALID_INPUT,
				Refresh));
	}
}