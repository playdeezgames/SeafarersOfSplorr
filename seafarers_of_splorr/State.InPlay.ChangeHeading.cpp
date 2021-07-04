#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.Update.h"
#include "Application.MouseButtonUp.h"
#include "Application.OnEnter.h"
#include "Common.Audio.h"
#include "Game.Audio.Mux.h"
#include "Game.Avatar.h"
#include "Visuals.Texts.h"
#include <format>
namespace state::in_play::ChangeHeading
{
	const std::string LAYOUT_NAME = "State.InPlay.ChangeHeading";
	const std::string TEXT_CURRENT_HEADING = "CurrentHeading";
	const std::string TEXT_NEW_HEADING = "NewHeading";
	static double newHeading = 0.0;

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::BACK, ::application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA) },
		{::Command::RED, ::application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA) }
	};

	static void UpdateHeadings()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_CURRENT_HEADING, std::format("Current: {}", game::Avatar::GetHeading()));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_NEW_HEADING, std::format("New: {}", game::Avatar::GetHeading()));
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		newHeading = game::Avatar::GetHeading();
		UpdateHeadings();
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_CHANGE_HEADING, OnEnter);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_CHANGE_HEADING, LAYOUT_NAME);
		::application::Command::SetHandlers(::UIState::IN_PLAY_CHANGE_HEADING, commandHandlers);
	}
}
