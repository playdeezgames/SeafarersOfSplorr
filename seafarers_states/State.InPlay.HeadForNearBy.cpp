#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Heading.h>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.Actions.h>
#include <Game.Islands.h>
#include <Game.Colors.h>
#include "State.InPlay.HeadForNearBy.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_HEAD_FOR_NEAR_BY;
	static const std::string LAYOUT_NAME = "State.Terminal";

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Head for:");
		Terminal::SetForeground(game::Colors::YELLOW);
		auto nearby = game::Islands::GetViewableIslands();
		if (!nearby.empty())
		{
			Terminal::WriteLine("Nearby islands:");
			int index = 1;
			for (auto& island : nearby)
			{
				Terminal::WriteLine("{}) {} ({:.2f} dist {:.1f})",
					index++,
					island.visits.has_value() ?
					island.name : "????", common::Heading::XYToDegrees(island.relativeLocation), island.relativeLocation.GetMagnitude());
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

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"0", application::UIState::GoTo(::UIState::IN_PLAY_CHANGE_HEADING)}
	};

	void HeadForNearBy::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				"Please make a valid selection.",
				Refresh));
	}
}