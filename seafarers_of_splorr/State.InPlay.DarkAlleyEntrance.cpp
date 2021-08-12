#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.OnEnter.h"
#include "Application.UIState.h"
#include "Game.Audio.Mux.h"
#include "Game.Avatar.h"
#include "Game.Avatar.Docked.h"
namespace state::in_play::DarkAlleyEntrance
{
	const std::string LAYOUT_NAME = "State.InPlay.DarkAlleyEntrance";

	static void OnLeave()
	{
		game::avatar::Docked::DoDockedAction(game::avatar::DockedAction::ENTER_DOCK);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		OnLeave();
		return true;
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_DARK_ALLEY_ENTRANCE, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_DARK_ALLEY_ENTRANCE, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::IN_PLAY_DARK_ALLEY_ENTRANCE, OnLeave);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_DARK_ALLEY_ENTRANCE, LAYOUT_NAME);
	}
}
