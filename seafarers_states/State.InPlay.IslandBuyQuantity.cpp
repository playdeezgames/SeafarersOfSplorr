#include <Common.Data.h>
#include <Game.Character.Docked.h>
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

		auto unitPrice = game::islands::Items::GetPurchasePrices(
			game::character::Docked::GetIsland(GetPlayerCharacterId()).value()
		)[currentItem];
		auto money = game::character::Statistics::ReadMoney(GetPlayerCharacterId());
		double availableTonnage = game::character::Ship::AvailableTonnage(GetPlayerCharacterId()).value();
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
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static void OnOtherInput(const std::string& line)
	{
		auto unitPrice = game::islands::Items::GetPurchasePrices(
				game::character::Docked::GetIsland(GetPlayerCharacterId()).value()
		)[currentItem];
		auto money = game::character::Statistics::ReadMoney(GetPlayerCharacterId());
		double availableTonnage = game::character::Ship::AvailableTonnage(GetPlayerCharacterId()).value();
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
			game::character::Statistics::ChangeMoney(GetPlayerCharacterId(), -totalPrice);
			game::islands::Markets::BuyItems(
					game::character::Docked::GetIsland(GetPlayerCharacterId()).value()
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