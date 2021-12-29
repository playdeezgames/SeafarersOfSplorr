#include <Common.Data.h>
#include <Common.Utility.Table.h>
#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.IslandSell.h"
#include "State.InPlay.IslandSellQuantity.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_ISLAND_SELL;

	static void OnLeave()
	{
		DoPlayerCharacterAction(game::characters::Action::ENTER_MARKET);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static const std::string FORMAT_MONEY = "Money: {:.3f}";

	static void RefreshStatistics()
	{
		auto currencyItem = game::Session().GetWorld().GetCurrencyItemSubtype();
		auto character = game::Session().GetPlayer().GetCharacter();
		auto markets = character.GetIsland().GetMarkets();
		auto quantity = character.GetItems().GetItemQuantity(currencyItem);
		auto money = quantity * markets.GetSaleValue(currencyItem);
		Terminal::WriteLine(FORMAT_MONEY, money);
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Sell:");
		Terminal::SetForeground(game::Colors::GRAY);
		RefreshStatistics();
		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		PlayMainTheme();
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "0", OnLeave}
	};

	static const void OnOtherInput(const std::string& line)
	{
		Terminal::ErrorMessage(Terminal::INVALID_INPUT);
		Refresh();
	}

	void IslandSell::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				OnOtherInput));
	}
}
