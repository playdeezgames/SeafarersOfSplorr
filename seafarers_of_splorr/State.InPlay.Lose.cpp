#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.OnEnter.h"
#include "Application.Renderer.h"
#include "Application.UIState.h"
#include "Game.Audio.Mux.h"
namespace state::in_play::Lose
{
	const std::string LAYOUT_NAME = "State.InPlay.Lose";

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		::application::UIState::Write(::UIState::MAIN_MENU);
		return true;
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_LOSE, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_LOSE, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::IN_PLAY_LOSE, ::application::UIState::GoTo(::UIState::MAIN_MENU));
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_LOSE, LAYOUT_NAME);
	}
}
