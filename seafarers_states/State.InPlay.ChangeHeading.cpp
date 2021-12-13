#include <Common.Heading.h>
#include "State.InPlay.ChangeHeading.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CHANGE_HEADING;

	static void Refresh()
	{
		Terminal::Reinitialize();

		auto playerCharacter =
			GetGameSession()
			.GetPlayerCharacter();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Change Heading:");
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Current: {:.2f}\xf8", 
			playerCharacter
			.GetBerth().value()
			.GetShip().value()
			.GetHeading().value());

		Terminal::SetForeground(game::Colors::YELLOW);
		if (playerCharacter
			.GetKnownIslands().HasAny())
		{
			Terminal::WriteLine("1) Head for a known island");
		}
		if (playerCharacter
			.GetBerth().value()
			.GetShip().value()
			.GetNearbyIslands().HasAny())
		{
			Terminal::WriteLine("2) Head for a nearby island");
		}
		if (playerCharacter
			.GetQuest().has_value())
		{
			Terminal::WriteLine("3) Head for job destination");
		}
		Terminal::WriteLine("4) Set heading manually");
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		PlayMainTheme();
		Refresh();
	}

	static void OnHeadForKnownIsland()
	{
		if (GetGameSession()
			.GetPlayerCharacter()
			.GetKnownIslands().HasAny())
		{
			application::UIState::Write(::UIState::IN_PLAY_HEAD_FOR_KNOWN);
		}
		else
		{
			Terminal::ErrorMessage(Terminal::INVALID_INPUT);
			Refresh();
		}
	}

	static void OnHeadForNearbyIsland()
	{
		if (GetGameSession()
			.GetPlayerCharacter()
			.GetBerth().value()
			.GetShip().value()
			.GetNearbyIslands().HasAny())
		{
			application::UIState::Write(::UIState::IN_PLAY_HEAD_FOR_NEAR_BY);
		}
		else
		{
			Terminal::ErrorMessage(Terminal::INVALID_INPUT);
			Refresh();
		}
	}

	static void OnHeadForJobDestination()
	{
		auto playerCharacter = GetGameSession().GetPlayerCharacter();
		auto quest = 
			playerCharacter
			.GetQuest();
		if (quest)
		{
			auto destination = quest.value().GetDestinationIsland().value().GetLocation().value();
			auto location = playerCharacter.GetBerth().value().GetShip().value().GetLocation().value();
			auto delta = destination - location;
			playerCharacter.GetBerth().value().GetShip().value().SetHeading(common::Heading::XYToDegrees(delta));
			Terminal::SetForeground(game::Colors::GREEN);
			Terminal::WriteLine();
			Terminal::WriteLine("You head for {}.", quest.value().GetDestinationIsland().value().GetDisplayName().value());
			application::UIState::Write(::UIState::IN_PLAY_NEXT);
		}
		else
		{
			Terminal::ErrorMessage("Please select a valid option.");
			Refresh();
		}
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", OnHeadForKnownIsland},
		{"2", OnHeadForNearbyIsland},
		{"3", OnHeadForJobDestination},
		{"4", application::UIState::GoTo(::UIState::IN_PLAY_MANUAL_HEADING)},
		{"0", application::UIState::GoTo(::UIState::IN_PLAY_SHIP_STATUS)}
	};

	void ChangeHeading::Start()
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