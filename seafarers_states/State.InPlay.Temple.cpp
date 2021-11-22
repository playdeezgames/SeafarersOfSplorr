#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Data.JSON.Stores.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Character.h>
#include <Game.Character.Actions.h>
#include <Game.Character.Docked.h>
#include <Game.Colors.h>
#include <Game.Islands.h>
#include <Game.Player.h>
#include "State.InPlay.Temple.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_TEMPLE;
	static const std::string FORMAT_TEMPLE = "Temple of {}";

	static void Refresh()
	{
		auto location = game::character::Docked::ReadLocation().value();
		auto island = game::Islands::Read(location).value();
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine(FORMAT_TEMPLE, island.patronDemigod);
		Terminal::WriteLine();

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Make offering");
		Terminal::WriteLine("0) Leave");

		Terminal::ShowPrompt();

	}

	static void OnMakeOffering()
	{
		::application::UIState::Write(::UIState::IN_PLAY_MAKE_OFFERING);
	}

	static void OnLeave()
	{
		game::character::Actions::DoAction(game::Player::GetAvatarId(), game::character::Action::ENTER_DOCK);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "1", OnMakeOffering },
		{ "0", OnLeave }
	};

	void Temple::Start()
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