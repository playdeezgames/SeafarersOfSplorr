#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.Quest.h>
#include <Game.Avatar.Log.h>
#include <Game.Colors.h>
#include "State.InPlay.ConfirmAbandonJob.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Menus.h>
namespace state::in_play//20211020
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_CONFIRM_ABANDON_JOB;

	static const std::string LAYOUT_NAME = "State.InPlay.ConfirmAbandonJob";
	static const std::string MENU_ID = "ConfirmAbandon";

	enum class ConfirmAbandonJobItem
	{
		NO,
		YES
	};

	static void AbandonJob()
	{
		game::avatar::Log::Write({
			game::Colors::RED,
			"You abandon yer job, and yer reputation suffers!"});
		game::avatar::Quest::Abandon();
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static const std::map<ConfirmAbandonJobItem, std::function<void()>> activators =
	{
		{ ConfirmAbandonJobItem::NO, ::application::UIState::GoTo(::UIState::IN_PLAY_NEXT) },
		{ ConfirmAbandonJobItem::YES, AbandonJob }
	};

	static const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	static const std::map<Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::GREEN, ActivateItem },
		{ ::Command::BACK, ::application::UIState::GoTo(::UIState::IN_PLAY_NEXT) },
		{ ::Command::RED, ::application::UIState::GoTo(::UIState::IN_PLAY_NEXT) }
	};

	void ConfirmAbandonJob::Start()
	{
		::application::OnEnter::AddHandler(
			CURRENT_STATE, 
			game::audio::Mux::GoToTheme(
				game::audio::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(
			CURRENT_STATE, 
			visuals::Areas::HandleMenuMouseButtonUp(
				LAYOUT_NAME, 
				ActivateItem));
		::application::MouseMotion::AddHandler(
			CURRENT_STATE, 
			visuals::Areas::HandleMenuMouseMotion(
				LAYOUT_NAME));
		::application::Command::SetHandlers(
			CURRENT_STATE, 
			commandHandlers);
		::application::Renderer::SetRenderLayout(
			CURRENT_STATE, 
			LAYOUT_NAME);
	}
}