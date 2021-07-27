#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.OnEnter.h"
#include "Game.Audio.Mux.h"
namespace state::in_play::ShipStatus
{
	const std::string LAYOUT_NAME = "State.InPlay.ShipStatus";

	static bool OnMouseButtonUp(const common::XY<int>& xy, unsigned char buttons)
	{
		::application::UIState::Pop();
		return true;
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_SHIP_STATUS, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_SHIP_STATUS, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::IN_PLAY_SHIP_STATUS, application::UIState::PopFrom());
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_SHIP_STATUS, LAYOUT_NAME);
	}
}
