#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Data.JSON.Stores.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.h>
#include <Game.Avatar.Actions.h>
#include <Game.Avatar.Docked.h>
#include <Game.Colors.h>
#include <Game.Islands.h>
#include "State.InPlay.IslandTrade.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_ISLAND_TRADE;

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Island Trading:");
		Terminal::WriteLine();

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("1) Buy");
		Terminal::WriteLine("2) Sell");
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();
	}

	static void OnBuy()
	{
		game::avatar::Actions::DoAction(game::avatar::Action::MARKET_BUY);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnSell()
	{
		game::avatar::Actions::DoAction(game::avatar::Action::MARKET_SELL);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnLeave()
	{
		game::avatar::Actions::DoAction(game::avatar::Action::ENTER_DOCK);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{"1", OnBuy},
		{"2", OnSell},
		{"0", OnLeave}
	};

	void IslandTrade::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				Terminal::INVALID_INPUT,
				Refresh));
	}
}