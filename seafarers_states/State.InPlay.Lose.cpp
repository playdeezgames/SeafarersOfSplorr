#include "State.InPlay.Globals.h"
#include "State.InPlay.Lose.h"
#include "State.MainMenu.h"
namespace state::in_play
{
	std::optional<int> Lose::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::RED);
		Terminal::WriteLine("YOU LOSE!");
		Terminal::WriteLine();
		Terminal::SetForeground(game::Colors::DARK_GRAY);
		Terminal::WriteLine("Press any key . . .");
	}

	static void OnEnter()
	{
		PlayMainTheme();
		Refresh();
	}

	static bool OnKey(const std::string&)
	{
		application::UIState::Write(MainMenu::GetStateId());
		return true;
	}

	void Lose::Start()
	{
		Registrar::Register(stateId, [](int currentState)
			{
				::application::OnEnter::AddHandler(currentState, OnEnter);
				::application::Renderer::SetRenderLayout(currentState, Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(currentState, OnKey);
			});
	}
}
