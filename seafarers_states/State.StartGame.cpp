#include <Game.Session.h>
#include "State.ChooseStartType.h"
#include "State.InPlay.Globals.h"
#include "State.LoadGame.h"
#include "State.MainMenu.h"
#include "State.Registrar.h"
#include "State.StartGame.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state
{
	std::optional<int> StartGame::stateId = std::nullopt;

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
		Terminal::menu.AddAction({ "Continue", ::application::UIState::DoGoTo(LoadGame::GetStateId) });
		Terminal::menu.AddAction({ "Easy", NewGame(game::Difficulty::EASY) });
		Terminal::menu.AddAction({ "Normal", NewGame(game::Difficulty::NORMAL) });
		Terminal::menu.AddAction({ "Hard", NewGame(game::Difficulty::HARD) });
		Terminal::menu.AddAction({ "HARDCORE", NewGame(game::Difficulty::HARDCORE) });
		MenuAction defaultAction = { "Never mind", application::UIState::DoGoTo(MainMenu::GetStateId) };
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
		Registrar::Register(stateId, [](int stateId) 
			{
				::application::OnEnter::AddHandler(
					stateId, 
					OnEnter);
				::application::Renderer::SetRenderLayout(
					stateId, 
					Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					stateId,
					Terminal::DoMenuInput(
						Terminal::INVALID_INPUT,
						Refresh));
			});
	}
}