#include <Common.Data.h>
#include <Game.Characters.Items.h>
#include <Game.Islands.Items.h>
#include <Game.Islands.Markets.h>
#include <Game.Items.h>
#include <Game.Session.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.IslandSellQuantity.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_ISLAND_SELL_QUANTITY;

	static void Refresh()
	{
		Terminal::Reinitialize();

		auto unitPrice = game::islands::Items::GetSalePrices(GetPlayerCharacterIslandId().value())[GetIslandTradeItem()];

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Selling {}:", game::Items::GetName(GetIslandTradeItem()));
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Unit price: {:.4f}.", unitPrice);
		Terminal::WriteLine("You have {} units.", game::characters::Items::Read(GetPlayerCharacterId(), GetIslandTradeItem()));

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		PlayMainTheme();
		Refresh();
	}

	static void OnOtherInput(const std::string& line)
	{
		auto unitPrice = game::islands::Items::GetSalePrices(GetPlayerCharacterIslandId().value())[GetIslandTradeItem()];
		int units = common::Data::ToInt(line);
		if (units <= game::characters::Items::Read(GetPlayerCharacterId(), GetIslandTradeItem()))
		{
			double totalPrice = unitPrice * units;
			Terminal::SetForeground(game::Colors::GREEN);
			Terminal::WriteLine("You sell {} {} for {:.4f}.", units, game::Items::GetName(GetIslandTradeItem()), totalPrice);
			auto currencyItem = game::Session().GetWorld().GetCurrencyItemSubtype();
			auto character = game::Session().GetPlayer().GetCharacter();
			auto markets = character.GetIsland().GetMarkets();
			auto quantity = markets.GetPurchaseQuantity(currencyItem, totalPrice);
			character.GetItems().AddItemQuantity(currencyItem, quantity);
			game::islands::Markets::SellItems(
				GetPlayerCharacterIslandId().value()
				, GetIslandTradeItem(), units);
			game::characters::Items::Remove(GetPlayerCharacterId(), GetIslandTradeItem(), units);
			application::UIState::Write(::UIState::IN_PLAY_ISLAND_SELL);
		}
		else
		{
			Terminal::ErrorMessage(Terminal::INVALID_INPUT);
			Refresh();
		}
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "0", application::UIState::GoTo(::UIState::IN_PLAY_NEXT)}
	};

	void IslandSellQuantity::Start()
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