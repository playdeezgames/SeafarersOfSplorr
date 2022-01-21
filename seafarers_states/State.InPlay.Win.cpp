#include "State.InPlay.Win.h"
#include "State.InPlay.Globals.h"
#include "State.MainMenu.h"
#include "State.Registrar.h"
namespace state::in_play
{
	std::optional<int> Win::stateId = std::nullopt;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::GREEN);
		Terminal::WriteLine("YOU WIN!");
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

	void Win::Start()
	{
		Registrar::Register(stateId, [](int currentState)
			{
				::application::OnEnter::AddHandler(currentState, OnEnter);
				::application::Renderer::SetRenderLayout(currentState, Terminal::LAYOUT_NAME);
				::application::Keyboard::AddHandler(currentState, OnKey);
			});
	}
}
