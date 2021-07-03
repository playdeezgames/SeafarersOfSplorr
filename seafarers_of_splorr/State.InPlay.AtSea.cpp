#include <string>
#include "Application.OnEnter.h"
#include "Game.Audio.Mux.h"
#include "Application.Renderer.h"
#include "Application.Command.h"
namespace state::in_play::AtSea 
{ 
	const std::string LAYOUT_NAME = "State.InPlay.AtSea";

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::RED, ::application::UIState::GoTo(::UIState::LEAVE_PLAY) }
	};


	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_AT_SEA, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_AT_SEA, LAYOUT_NAME);
		::application::Command::SetHandlers(::UIState::IN_PLAY_AT_SEA, commandHandlers);
	}
}