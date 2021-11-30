#include <Common.Data.h>
#include <Game.Character.Docked.h>
#include <Game.Character.Items.h>
#include <Game.Character.Statistics.h>
#include <Game.Islands.Items.h>
#include <Game.Islands.Markets.h>
#include <Game.Items.h>
#include "State.InPlay.Globals.h"
#include "State.InPlay.IslandSellQuantity.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_ISLAND_SELL_QUANTITY;
	static game::Item currentItem;

	static void Refresh()
	{
		Terminal::Reinitialize();

		auto unitPrice = game::islands::Items::GetSalePrices(
			GetPlayerCharacterIslandId().value()
		)[currentItem];

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Selling {}:", game::Items::GetName(currentItem));
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Unit price: {:.4f}.", unitPrice);
		Terminal::WriteLine("You have {} units.", game::character::Items::Read(GetPlayerCharacterId(), currentItem));

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static void OnOtherInput(const std::string& line)
	{
		auto unitPrice = game::islands::Items::GetSalePrices(
			GetPlayerCharacterIslandId().value()
		)[currentItem];
		int units = common::Data::ToInt(line);
		if (units <= game::character::Items::Read(GetPlayerCharacterId(), currentItem))
		{
			double totalPrice = unitPrice * units;
			Terminal::SetForeground(game::Colors::GREEN);
			Terminal::WriteLine("You sell {} {} for {:.4f}.", units, game::Items::GetName(currentItem), totalPrice);
			game::character::Statistics::ChangeMoney(GetPlayerCharacterId(), totalPrice);
			game::islands::Markets::SellItems(
				GetPlayerCharacterIslandId().value()
				, currentItem, units);
			game::character::Items::Remove(GetPlayerCharacterId(), currentItem, units);
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

	void IslandSellQuantity::SetItem(const game::Item& item)
	{
		currentItem = item;
	}

}