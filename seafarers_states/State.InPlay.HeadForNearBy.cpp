#include <Common.Heading.h>
#include <Common.Utility.List.h>
#include <Game.Ship.h>
#include <Game.Islands.h>
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
		auto nearby = game::Islands::GetViewableIslands(GetPlayerCharacterShipId().value());
		if (!nearby.empty())
		{
			Terminal::SetForeground(game::Colors::GRAY);
			Terminal::WriteLine("Nearby islands:");
			Terminal::SetForeground(game::Colors::YELLOW);
			int index = 1;
			for (auto& island : nearby)
			{
				Terminal::WriteLine("{}) {} ({:.2f}\xf8 dist {:.1f})",
					index++,
					island.GetDisplayName(), 
					common::Heading::XYToDegrees(island.relativeLocation), 
					island.relativeLocation.GetMagnitude());
			}
		}

		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static std::function<void()> DoHeadForNearByIndex(size_t index)
	{
		return [index]() 
		{
			auto nearby = game::Islands::GetViewableIslands(GetPlayerCharacterShipId().value());
			auto chosen = common::utility::List::GetNth(nearby, index);
			if (chosen)
			{
				Terminal::SetForeground(game::Colors::GREEN);
				Terminal::WriteLine();
				game::Ship::SetHeading(GetPlayerCharacterShipId().value(), common::Heading::XYToDegrees(chosen.value().relativeLocation));
				Terminal::WriteLine("You head for {}.", chosen.value().GetDisplayName());
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