#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.Terminal.h"
#include "State.StartGame.h"
#include "State.ChooseStartType.h"
#include "UIState.h"
namespace state
{
	static const ::UIState CURRENT_STATE = ::UIState::START_GAME;

	static std::function<void()> NewGame(const game::Difficulty& difficulty)
	{
		return [difficulty]()
		{
			game::Session().Reset();
			game::Session().Populate(difficulty);
			application::UIState::Write(ChooseStartType::GetStateId());
		};
	}

	static void Refresh()
	{
		Terminal::ClearStatusLine();
		Terminal::ClearInput();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine();
		Terminal::WriteLine("Start Game:");

		Terminal::ShowMenu();

		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine();
		Terminal::Write(">");
	}

	static void UpdateMenu()
	{
		Terminal::menu.Clear();
		Terminal::menu.SetRefresh(Refresh);
		Terminal::menu.AddAction({ "Continue", ::application::UIState::GoTo(::UIState::LOAD_GAME) });
		Terminal::menu.AddAction({ "Easy", NewGame(game::Difficulty::EASY) });
		Terminal::menu.AddAction({ "Normal", NewGame(game::Difficulty::NORMAL) });
		Terminal::menu.AddAction({ "Hard", NewGame(game::Difficulty::HARD) });
		Terminal::menu.AddAction({ "HARDCORE", NewGame(game::Difficulty::HARDCORE) });
		MenuAction defaultAction = { "Never mind", application::UIState::GoTo(::UIState::MAIN_MENU) };
		Terminal::menu.SetDefaultAction(defaultAction);
	}

	static void OnEnter()
	{
		in_play::PlayMainTheme();
		UpdateMenu();
		Refresh();
	}

	void StartGame::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoMenuInput(
				Terminal::INVALID_INPUT,
				Refresh));
	}
}