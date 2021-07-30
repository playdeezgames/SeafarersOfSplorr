#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.OnEnter.h"
#include "Application.Renderer.h"
#include "Application.UIState.h"
#include "Common.Audio.h"
#include "Common.Utility.h"
#include "Game.Audio.Mux.h"
#include "Visuals.Areas.h"
#include "Visuals.Menus.h"
namespace state::LeavePlay
{
	const std::string LAYOUT_NAME = "State.LeavePlay";
	const std::string MENU_ID = "LeavePlay";

	enum class LeavePlayItem
	{
		CONTINUE,
		SAVE,
		OPTIONS,
		ABANDON
	};

	static void GoToConfirmAbandon()
	{
		::application::UIState::Write(::UIState::CONFIRM_ABANDON);
	}

	static void GoToSaveGame()
	{
		::application::UIState::Write(::UIState::SAVE_GAME);
	}

	static void ContinueGame()
	{
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	const std::map<LeavePlayItem, std::function<void()>> activators =
	{
		{ LeavePlayItem::ABANDON, GoToConfirmAbandon },
		{ LeavePlayItem::SAVE, GoToSaveGame },
		{ LeavePlayItem::CONTINUE, ContinueGame },
		{ LeavePlayItem::OPTIONS, application::UIState::PushTo(::UIState::OPTIONS) }
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (LeavePlayItem)visuals::Menus::ReadIndex(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::GREEN, ActivateItem },
		{ ::Command::BACK, ContinueGame },
		{ ::Command::RED, ContinueGame }
	};

	static void SetCurrentMenuItem(LeavePlayItem item)
	{
		visuals::Menus::WriteIndex(LAYOUT_NAME, MENU_ID, (int)item);
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::LEAVE_PLAY, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::LEAVE_PLAY, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::MouseMotion::AddHandler(::UIState::LEAVE_PLAY, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::Command::SetHandlers(::UIState::LEAVE_PLAY, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::LEAVE_PLAY, LAYOUT_NAME);
	}
}