#include <Common.Data.h>
#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.MultipleMove.h"
#include "State.Registrar.h"
namespace state::in_play
{
	std::optional<int> MultipleMove::stateId = std::nullopt;

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
		Registrar::Register(stateId, [](int stateId) 
			{
				::application::OnEnter::AddHandler(stateId, OnEnter);
				::application::Renderer::SetRenderLayout(stateId, Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					stateId,
					Terminal::DoIntegerInput(
						menuActions, 
						OnOtherInput));
			});
	}
}