#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.Renderer.h"
#include "Common.Utility.h"
#include "Visuals.Areas.h"
#include "Visuals.Menus.h"
#include "Application.OnEnter.h"
#include "Common.Audio.h"
#include "Game.Audio.Mux.h"
#include "Visuals.Texts.h"
#include "Data.Stores.h"
#include "Game.Avatar.h"
#include "Game.Islands.h"
#include <format>
namespace state::in_play::IslandTrade
{
	const std::string LAYOUT_NAME = "State.InPlay.IslandTrade";
	const std::string MENU_ID = "Trade";

	enum class TradeMenuItem
	{
		BUY,
		SELL,
		LEAVE
	};

	static void OnSell()
	{
		return;
	}

	const std::map<TradeMenuItem, std::function<void()>> activators =
	{
		{ TradeMenuItem::BUY, ::application::UIState::GoTo(::UIState::IN_PLAY_ISLAND_BUY) },
		{ TradeMenuItem::SELL, OnSell },
		{ TradeMenuItem::LEAVE, ::application::UIState::GoTo(::UIState::IN_PLAY_DOCKED) }
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (TradeMenuItem)visuals::Menus::ReadIndex(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{::Command::GREEN, ActivateItem },
		{::Command::BACK, ::application::UIState::GoTo(::UIState::IN_PLAY_DOCKED) },
		{::Command::RED, ::application::UIState::GoTo(::UIState::IN_PLAY_DOCKED) }
	};

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		auto location = game::Avatar::GetDockedLocation().value();
		auto island = game::Islands::Read(location).value();
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_ISLAND_TRADE, OnEnter);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_ISLAND_TRADE, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_ISLAND_TRADE, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::Command::SetHandlers(::UIState::IN_PLAY_ISLAND_TRADE, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_ISLAND_TRADE, LAYOUT_NAME);
	}
}