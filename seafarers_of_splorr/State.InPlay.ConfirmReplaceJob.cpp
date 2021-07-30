#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.OnEnter.h"
#include "Application.Renderer.h"
#include "Application.UIState.h"
#include "Common.Utility.h"
#include "Game.Audio.Mux.h"
#include "Game.Avatar.Quest.h"
#include "Game.Avatar.h"
#include "Visuals.Areas.h"
#include "Visuals.Menus.h"
namespace state::in_play::ConfirmReplaceJob
{
	const std::string LAYOUT_NAME = "State.InPlay.ConfirmReplaceJob";
	const std::string MENU_ID = "ConfirmReplace";

	enum class ConfirmReplaceJobItem
	{
		NO,
		YES
	};

	static void ReplaceJob()
	{
		game::avatar::Quest::AbandonQuest();
		game::avatar::Quest::AcceptQuest(game::Avatar::GetDockedLocation().value());
		::application::UIState::Write(::UIState::IN_PLAY_DOCKED);
	}

	const std::map<ConfirmReplaceJobItem, std::function<void()>> activators =
	{
		{ ConfirmReplaceJobItem::NO, ::application::UIState::GoTo(::UIState::IN_PLAY_ISLAND_JOBS) },
		{ ConfirmReplaceJobItem::YES, ReplaceJob }
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (ConfirmReplaceJobItem)visuals::Menus::ReadIndex(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::GREEN, ActivateItem },
		{ ::Command::BACK, ::application::UIState::GoTo(::UIState::IN_PLAY_ISLAND_JOBS) },
		{ ::Command::RED, ::application::UIState::GoTo(::UIState::IN_PLAY_ISLAND_JOBS) }
	};

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_CONFIRM_REPLACE_JOB, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_CONFIRM_REPLACE_JOB, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_CONFIRM_REPLACE_JOB, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::Command::SetHandlers(::UIState::IN_PLAY_CONFIRM_REPLACE_JOB, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_CONFIRM_REPLACE_JOB, LAYOUT_NAME);
	}
}