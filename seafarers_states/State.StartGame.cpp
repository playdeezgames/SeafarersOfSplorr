#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Game.h>
#include <Game.Colors.h>
#include <Game.Audio.Mux.h>
#include "State.Terminal.h"
#include "States.h"
#include "UIState.h"
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::START_GAME;
	static const std::string LAYOUT_NAME = "State.Terminal";

	static std::function<void()> NewGame(const game::Difficulty& difficulty)
	{
		return [difficulty]()
		{
			game::Reset(difficulty);
			application::UIState::Write(::UIState::TIP);
		};
	}

	static void Refresh()
	{
		Terminal::ClearStatusLine();
		Terminal::ClearInput();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine();
		Terminal::WriteLine("Start Game:");

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine();
		Terminal::WriteLine("1) Continue");
		Terminal::WriteLine("2) Easy");
		Terminal::WriteLine("3) Normal");
		Terminal::WriteLine("4) Hard");
		Terminal::WriteLine("5) HARDCORE");
		Terminal::WriteLine("6) Never mind");

		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine();
		Terminal::Write(">");
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "1", ::application::UIState::GoTo(::UIState::LOAD_GAME) },
		{ "2", NewGame(game::Difficulty::EASY)},
		{ "3", NewGame(game::Difficulty::NORMAL) },
		{ "4", NewGame(game::Difficulty::HARD) },
		{ "5", NewGame(game::Difficulty::HARDCORE) },
		{ "6", ::application::UIState::GoTo(::UIState::MAIN_MENU) }
	};

	void StartGame::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
		::application::Keyboard::AddHandler(CURRENT_STATE, Terminal::DoIntegerInput(menuActions, "Please enter a number between 1 and 6", Refresh));

	}
}