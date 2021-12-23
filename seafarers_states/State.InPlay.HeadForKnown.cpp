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

	static void OnEnter()
	{
		PlayMainTheme();
		Refresh();
	}

	static void DoHeadForKnownIndex(size_t index)
	{
		auto character = game::Session().GetPlayer().GetCharacter();
		auto location = character.GetBerth().GetShip().GetLocation();
		auto nearby = character.GetKnownIslands().GetAll();
		auto chosen = common::utility::List::GetNth(nearby, index);
		if (chosen)
		{
			auto relativeLocation = chosen.value().GetIsland().GetLocation() - location;
			Terminal::SetForeground(game::Colors::GREEN);
			Terminal::WriteLine();
			game::Session().GetPlayer().GetCharacter().GetBerth().GetShip().SetHeading(common::Heading::XYToDegrees(relativeLocation));
			Terminal::WriteLine("You head for {}.", chosen.value().GetDisplayName());
			application::UIState::Write(::UIState::IN_PLAY_AT_SEA_OVERVIEW);
		}
		else
		{
			Terminal::SetForeground(game::Colors::RED);
			Terminal::WriteLine();
			Terminal::WriteLine("Please select a valid option.");
			Refresh();
		}
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"0", application::UIState::GoTo(::UIState::IN_PLAY_CHANGE_HEADING)}
	};

	static void OnOtherInput(const std::string& line)
	{
		DoHeadForKnownIndex((size_t)common::Data::ToInt(line)- (size_t)1);
	}

	void HeadForKnown::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				OnOtherInput));
	}
}