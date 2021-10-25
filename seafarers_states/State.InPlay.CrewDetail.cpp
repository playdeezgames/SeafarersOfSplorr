#include <Application.Command.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include "State.InPlay.CrewDetail.h"
#include <string>
#include "UIState.h"
namespace state::in_play//20211019
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CREW_DETAIL;
	static const std::string LAYOUT_NAME = "State.InPlay.CrewDetail";

	static int currentAvatarId;

	void CrewDetail::SetAvatarId(int avatarId)
	{
		currentAvatarId = avatarId;
	}

	static auto OnLeave = application::UIState::GoTo(::UIState::IN_PLAY_CREW_LIST);

	static void Refresh()
	{
		//TODO: don't worry... something will be here... this is a WIP
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::BACK, OnLeave },
		{ ::Command::RED, OnLeave }
	};

	void CrewDetail::Start()
	{
		::application::OnEnter::AddHandler(
			CURRENT_STATE, 
			OnEnter);
		::application::Command::SetHandlers(
			CURRENT_STATE, 
			commandHandlers);
		::application::Renderer::SetRenderLayout(
			CURRENT_STATE, 
			LAYOUT_NAME);
	}
}