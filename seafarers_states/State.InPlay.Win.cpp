#include "State.InPlay.Win.h"
#include "State.InPlay.Globals.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_WIN;

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

	static bool OnKey(const std::string& key)
	{
		application::UIState::Write(::UIState::MAIN_MENU);
		return true;
	}

	void Win::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(CURRENT_STATE, OnKey);
	}
}
