#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include <Game.Session.Saves.h>
#include "State.ConfirmOverwrite.h"
#include "State.InPlay.Next.h"
#include "State.SaveGame.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state
{
	static const std::map<::UIState, int> slotTable =
	{
		{::UIState::CONFIRM_OVERWRITE_SLOT1, 1},
		{::UIState::CONFIRM_OVERWRITE_SLOT2, 2},
		{::UIState::CONFIRM_OVERWRITE_SLOT3, 3},
		{::UIState::CONFIRM_OVERWRITE_SLOT4, 4},
		{::UIState::CONFIRM_OVERWRITE_SLOT5, 5},
	};

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Are you sure you want to overwrite that save slot?");
		Terminal::WriteLine();

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Yes");
		Terminal::WriteLine("0) No");

		Terminal::ShowPrompt();
	}

	static void ConfirmOverwrite()
	{
		switch ((::UIState)application::UIState::Read())
		{
		case ::UIState::CONFIRM_OVERWRITE_SLOT1:
			game::session::Saves().GetSlot1().Store();
			break;
		case ::UIState::CONFIRM_OVERWRITE_SLOT2:
			game::session::Saves().GetSlot2().Store();
			break;
		case ::UIState::CONFIRM_OVERWRITE_SLOT3:
			game::session::Saves().GetSlot3().Store();
			break;
		case ::UIState::CONFIRM_OVERWRITE_SLOT4:
			game::session::Saves().GetSlot4().Store();
			break;
		case ::UIState::CONFIRM_OVERWRITE_SLOT5:
			game::session::Saves().GetSlot5().Store();
			break;
		}
		application::UIState::Write(in_play::Next::GetStateId());
	}

	static const std::vector<::UIState> states =
	{
		::UIState::CONFIRM_OVERWRITE_SLOT1,
		::UIState::CONFIRM_OVERWRITE_SLOT2,
		::UIState::CONFIRM_OVERWRITE_SLOT3,
		::UIState::CONFIRM_OVERWRITE_SLOT4,
		::UIState::CONFIRM_OVERWRITE_SLOT5,
	};

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "0", application::UIState::DoGoTo(SaveGame::GetStateId) },
		{ "1", ConfirmOverwrite }
	};

	static void ConfigureState(const ::UIState& state)
	{
		::application::OnEnter::AddHandler(state, OnEnter);
		::application::Renderer::SetRenderLayout(state, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(state, Terminal::DoIntegerInput(menuActions, "Please enter a number between 1 and 2.", OnEnter));
	}

	void ConfirmOverwrite::Start()
	{
		for (auto state : states)
		{
			ConfigureState(state);
		}
	}
}