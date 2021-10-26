#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include <Game.Colors.h>
#include "State.InPlay.ChangeSpeed.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CHANGE_SPEED;
	static const std::string LAYOUT_NAME = "State.Terminal";

	static void Refresh()
	{
		Terminal::ClearStatusLine();

		Terminal::ClearInput();
		Terminal::WriteLine();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Change Speed:");
		Terminal::WriteLine();

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) All stop");
		Terminal::WriteLine("2) Ahead 1/3");
		Terminal::WriteLine("3) Ahead 2/3");
		Terminal::WriteLine("4) Ahead full");
		Terminal::WriteLine("5) Ahead flank");
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
		{"6", application::UIState::GoTo(::UIState::IN_PLAY_STEER_SHIP)}
	};

	void ChangeSpeed::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE, 
			Terminal::DoIntegerInput(
				menuActions, 
				"Please enter a number between 1 and 6.", 
				Refresh));
	}
}