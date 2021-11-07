#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include "State.InPlay.Lose.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_LOSE;

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
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static bool OnKey(const std::string& key)
	{
		application::UIState::Write(::UIState::MAIN_MENU);
		return true;
	}

	void Lose::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(CURRENT_STATE, OnKey);
	}
}
