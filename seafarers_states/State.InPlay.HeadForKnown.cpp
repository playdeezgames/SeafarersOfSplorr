#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Data.h>
#include <Common.Heading.h>
#include <Common.Utility.List.h>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.Actions.h>
#include <Game.Avatar.Ship.h>
#include <Game.Ship.h>
#include <Game.Islands.h>
#include <Game.Colors.h>
#include <Game.Player.h>
#include "State.InPlay.HeadForKnown.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_HEAD_FOR_KNOWN;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Head for:");
		auto known = game::Islands::GetKnownIslands(game::Ship::GetLocation(game::avatar::Ship::ReadShipId(game::Player::GetAvatarId()).value()).value());
		if (!known.empty())
		{
			Terminal::SetForeground(game::Colors::GRAY);
			Terminal::WriteLine("Known islands:");
			Terminal::SetForeground(game::Colors::YELLOW);
			int index = 1;
			for (auto& island : known)
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

	static void DoHeadForKnownIndex(size_t index)
	{
		auto nearby = game::Islands::GetKnownIslands(game::Ship::GetLocation(game::avatar::Ship::ReadShipId(game::Player::GetAvatarId()).value()).value());
		auto chosen = common::utility::List::GetNth(nearby, index);
		if (chosen)
		{
			Terminal::SetForeground(game::Colors::GREEN);
			Terminal::WriteLine();
			game::Ship::SetHeading(game::avatar::Ship::ReadShipId(game::Player::GetAvatarId()).value(), common::Heading::XYToDegrees(chosen.value().relativeLocation));
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
		DoHeadForKnownIndex(common::Data::ToInt(line)-1);
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