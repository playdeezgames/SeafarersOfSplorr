#include <Common.Data.h>
#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.MultipleMove.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_MULTIPLE_MOVE;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("How many moves?");
		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		PlayMainTheme();
		Refresh();
	}

	static void OnOtherInput(const std::string line)
	{
		int moves = common::Data::ToInt(line);
		while (moves > 0)
		{
			Terminal::SetForeground(game::Colors::GREEN);
			Terminal::WriteLine();
			Terminal::WriteLine("Steady as she goes!");
			game::Session().ApplyTurnEffects();
			moves--;
		}
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"0", application::UIState::GoTo(::UIState::IN_PLAY_NEXT)}
	};


	void MultipleMove::Start()
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