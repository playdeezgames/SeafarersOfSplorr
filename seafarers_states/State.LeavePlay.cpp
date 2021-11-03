#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include "State.LeavePlay.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::LEAVE_PLAY;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Game Menu:");
		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Abandon Game");
		Terminal::WriteLine("2) Save Game");
		Terminal::WriteLine("3) Options");
		Terminal::WriteLine("0) Continue Game");

		Terminal::ShowPrompt();
	}

	static void GoToConfirmAbandon()
	{
		::application::UIState::Write(::UIState::CONFIRM_ABANDON);
	}

	static void GoToSaveGame()
	{
		::application::UIState::Write(::UIState::SAVE_GAME);
	}

	static void ContinueGame()
	{
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", GoToConfirmAbandon},
		{"2", GoToSaveGame},
		{"3", application::UIState::PushTo(::UIState::OPTIONS)},
		{"0", ContinueGame}
	};

	void LeavePlay::Start()
	{
		::application::OnEnter::AddHandler(::UIState::LEAVE_PLAY, OnEnter);
		::application::Renderer::SetRenderLayout(::UIState::LEAVE_PLAY, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				"Please enter a number between 1 and 4.",
				Refresh));

	}
}