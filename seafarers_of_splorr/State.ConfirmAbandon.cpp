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
namespace state::ConfirmAbandon
{
	const std::string LAYOUT_NAME = "State.ConfirmAbandon";
	const std::string MENU_ID = "ConfirmAbandon";

	enum class ConfirmAbandonItem
	{
		NO,
		YES
	};

	static void CancelAbandon()
	{
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	const std::map<ConfirmAbandonItem, std::function<void()>> activators =
	{
		{ ConfirmAbandonItem::NO, CancelAbandon },
		{ ConfirmAbandonItem::YES, ::application::UIState::GoTo(::UIState::MAIN_MENU) }
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (ConfirmAbandonItem)visuals::Menus::ReadIndex(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::GREEN, ActivateItem },
		{ ::Command::BACK, ::application::UIState::GoTo(::UIState::MAIN_MENU) },
		{ ::Command::RED, ::application::UIState::GoTo(::UIState::MAIN_MENU) }
	};

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::CONFIRM_ABANDON, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::CONFIRM_ABANDON, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::MouseMotion::AddHandler(::UIState::CONFIRM_ABANDON, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::Command::SetHandlers(::UIState::CONFIRM_ABANDON, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::CONFIRM_ABANDON, LAYOUT_NAME);
	}
}