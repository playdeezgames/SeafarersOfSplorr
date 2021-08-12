#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.OnEnter.h"
#include "Application.Renderer.h"
#include "Application.UIState.h"
#include "Common.Audio.h"
#include "Common.Utility.h"
#include "Data.Stores.h"
#include <format>
#include "Game.Audio.Mux.h"
#include "Game.Avatar.h"
#include "Game.Islands.h"
#include "Visuals.Areas.h"
#include "Visuals.Menus.h"
#include "Visuals.Texts.h"
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

	static void OnBuy()
	{
		game::Avatar::DoDockedAction(game::avatar::DockedAction::MARKET_BUY);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnSell()
	{
		game::Avatar::DoDockedAction(game::avatar::DockedAction::MARKET_SELL);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnLeave()
	{
		game::Avatar::DoDockedAction(game::avatar::DockedAction::ENTER_DOCK);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	const std::map<TradeMenuItem, std::function<void()>> activators =
	{
		{ TradeMenuItem::BUY, OnBuy },
		{ TradeMenuItem::SELL, OnSell },
		{ TradeMenuItem::LEAVE, OnLeave }
	};

	const auto ActivateItem = visuals::Menus::DoActivateItem(LAYOUT_NAME, MENU_ID, activators);

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{::Command::GREEN, ActivateItem },
		{::Command::BACK, OnLeave },
		{::Command::RED, OnLeave }
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