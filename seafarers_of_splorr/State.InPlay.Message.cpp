#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.OnEnter.h"
#include "Application.UIState.h"
#include "Game.Audio.Mux.h"
namespace state::in_play::Message
{
	const std::string LAYOUT_NAME = "State.InPlay.Message";

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
		return true;
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_MESSAGE, OnEnter);
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_MESSAGE, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::IN_PLAY_MESSAGE, ::application::UIState::GoTo(::UIState::IN_PLAY_NEXT));
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_MESSAGE, LAYOUT_NAME);
	}
}
