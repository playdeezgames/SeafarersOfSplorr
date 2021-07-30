#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.OnEnter.h"
#include "Application.Renderer.h"
#include "Application.UIState.h"
#include "Game.Audio.Mux.h"
namespace state::in_play::Win
{
	const std::string LAYOUT_NAME = "State.InPlay.Win";

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		::application::UIState::Write(::UIState::MAIN_MENU);
		return true;
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_WIN, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_WIN, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::IN_PLAY_WIN, ::application::UIState::GoTo(::UIState::MAIN_MENU));
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_WIN, LAYOUT_NAME);
	}
}
