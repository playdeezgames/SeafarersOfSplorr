#include <Common.Data.h>
#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.MultipleMove.h"
#include "State.InPlay.Next.h"
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
		application::UIState::Write(Next::GetStateId());
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"0", application::UIState::DoGoTo(Next::GetStateId)}
	};

	void MultipleMove::Start()
	{
		Registrar::Register(stateId, [](int currentState)
			{
				::application::OnEnter::AddHandler(currentState, OnEnter);
				::application::Renderer::SetRenderLayout(currentState, Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(
					currentState,
					Terminal::DoIntegerInput(
						menuActions, 
						OnOtherInput));
			});
	}
}