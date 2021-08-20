#include <Application.Renderer.h>
#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.OnEnter.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.Docked.h>
#include "UIState.h"
namespace state::in_play::GambleStart
{
	const std::string LAYOUT_NAME = "State.InPlay.GambleStart";

	static void OnLeave()
	{
		game::avatar::Docked::DoDockedAction(game::avatar::DockedAction::ENTER_DARK_ALLEY);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		OnLeave();
		return true;
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_GAMBLE_START, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_GAMBLE_START, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::IN_PLAY_GAMBLE_START, OnLeave);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_GAMBLE_START, LAYOUT_NAME);
	}
}
