#include <string>
#include "Application.OnEnter.h"
#include "Game.Audio.Mux.h"
#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Game.Avatar.h"
#include "Visuals.Texts.h"
#include <format>
namespace state::in_play::AtSea 
{ 
	const std::string LAYOUT_NAME = "State.InPlay.AtSea";
	const std::string TEXT_AVATAR_X = "avatar-x";
	const std::string TEXT_AVATAR_Y = "avatar-y";
	const std::string TEXT_AVATAR_HEADING = "avatar-heading";
	const std::string TEXT_AVATAR_SPEED = "avatar-speed";

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::RED, ::application::UIState::GoTo(::UIState::LEAVE_PLAY) }
	};

	void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		auto location = game::Avatar::GetLocation();
		auto heading = game::Avatar::GetHeading();
		auto speed = game::Avatar::GetSpeed();
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_X, std::format("X: {}", location.GetX()));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_Y, std::format("Y: {}", location.GetY()));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_HEADING, std::format("Heading: {}", heading));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_AVATAR_SPEED, std::format("Speed: {}", speed));
	}


	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_AT_SEA, OnEnter);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_AT_SEA, LAYOUT_NAME);
		::application::Command::SetHandlers(::UIState::IN_PLAY_AT_SEA, commandHandlers);
	}
}