#include <Common.Data.h>
#include <Game.Character.Items.h>
#include <Game.Character.Ship.h>
#include <Game.Character.Statistics.h>
#include <Game.Islands.Items.h>
#include <Game.Islands.Markets.h>
#include <Game.Items.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.IslandBuyQuantity.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_ISLAND_BUY_QUANTITY;
	static game::Item currentItem;

	static void Refresh()
	{
		Terminal::Reinitialize();

		auto unitPrice = GetPlayerCharacterPurchasePrices().value()[currentItem];
		auto money = GetPlayerCharacterMoney();
		double availableTonnage = GetPlayerCharacterAvailableTonnage().value();
		double unitTonnage = game::Items::GetUnitTonnage(currentItem);
		int affordableQuantity = (int)(money / unitPrice);
		int storableQuantity = (int)(availableTonnage / unitTonnage);
		int maximumQuantity = std::min(affordableQuantity, storableQuantity);

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Buying {}:", game::Items::GetName(currentItem));
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Unit price: {:.4f}.", unitPrice);
		Terminal::WriteLine("Current funds: {:.4f}", money);
		Terminal::WriteLine("You can afford to buy {} unit.", maximumQuantity);

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		PlayMainTheme();
		Refresh();
	}

	static void OnOtherInput(const std::string& line)
	{
		auto unitPrice = GetPlayerCharacterPurchasePrices().value()[currentItem];
		auto money = GetPlayerCharacterMoney();
		double availableTonnage = GetPlayerCharacterAvailableTonnage().value();
		double unitTonnage = game::Items::GetUnitTonnage(currentItem);
		int affordableQuantity = (int)(money / unitPrice);
		int storableQuantity = (int)(availableTonnage / unitTonnage);
		int maximumQuantity = std::min(affordableQuantity, storableQuantity);
		int units = common::Data::ToInt(line);
		if (units <= maximumQuantity)
		{
			double totalPrice = unitPrice * units;
			Terminal::SetForeground(game::Colors::GREEN);
			Terminal::WriteLine("You purchase {} {} for {:.4f}.", units, game::Items::GetName(currentItem), totalPrice);
			ChangePlayerCharacterMoney(-totalPrice);
			game::islands::Markets::BuyItems(
					GetPlayerCharacterIslandId().value()
				, currentItem, units);
			game::character::Items::Add(GetPlayerCharacterId(), currentItem, units);
			application::UIState::Write(::UIState::IN_PLAY_ISLAND_BUY);
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

	void IslandBuyQuantity::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				OnOtherInput));
	}

	void IslandBuyQuantity::SetItem(const game::Item& item)
	{
		currentItem = item;
	}
}