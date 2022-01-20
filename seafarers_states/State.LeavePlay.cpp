#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include "State.ConfirmAbandon.h"
#include "State.InPlay.Next.h"
#include "State.LeavePlay.h"
#include "State.Options.h"
#include "State.Registrar.h"
#include "State.SaveGame.h"
#include "State.Terminal.h"
namespace state
{
	std::optional<int> LeavePlay::stateId = std::nullopt;

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
		::application::UIState::Write(ConfirmAbandon::GetStateId());
	}

	static void GoToSaveGame()
	{
		::application::UIState::Write(SaveGame::GetStateId());
	}

	static void ContinueGame()
	{
		application::UIState::Write(in_play::Next::GetStateId());
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
		{"3", application::UIState::DoPushTo(Options::GetStateId)},
		{"0", ContinueGame}
	};

	void LeavePlay::Start()
	{
		Registrar::Register(stateId, [](int stateId) 
			{
				::application::OnEnter::AddHandler(stateId, OnEnter);
				::application::Renderer::SetRenderLayout(stateId, Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					stateId,
					Terminal::DoIntegerInput(
						menuActions,
						Terminal::INVALID_INPUT,
						Refresh));
			});
	}
}