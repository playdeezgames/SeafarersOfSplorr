#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Data.h>
#include <Common.Utility.Dispatcher.h>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.Docked.h>
#include <Game.Avatar.Items.h>
#include <Game.Avatar.Ship.h>
#include <Game.Avatar.Statistics.h>
#include <Game.Colors.h>
#include <Game.Islands.Items.h>
#include <Game.Islands.Markets.h>
#include <Game.Items.h>
#include <Game.Player.h>
#include "State.InPlay.IslandSellQuantity.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_ISLAND_SELL_QUANTITY;
	static game::Item currentItem;

	static void Refresh()
	{
		Terminal::Reinitialize();

		auto unitPrice = game::islands::Items::GetSalePrices(game::avatar::Docked::ReadLocation().value())[currentItem];

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Selling {}:", game::Items::GetName(currentItem));
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("Unit price: {:.4f}.", unitPrice);
		Terminal::WriteLine("You have {} units.", game::avatar::Items::Read(currentItem));

		Terminal::ShowPrompt();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static void OnOtherInput(const std::string& line)
	{
		auto unitPrice = game::islands::Items::GetSalePrices(game::avatar::Docked::ReadLocation().value())[currentItem];
		int units = common::Data::ToInt(line);
		if (units <= game::avatar::Items::Read(currentItem))
		{
			double totalPrice = unitPrice * units;
			Terminal::SetForeground(game::Colors::GREEN);
			Terminal::WriteLine("You sell {} {} for {:.4f}.", units, game::Items::GetName(currentItem), totalPrice);
			game::avatar::Statistics::ChangeMoney(game::Player::GetAvatarId(), totalPrice);
			game::islands::Markets::SellItems(game::avatar::Docked::ReadLocation().value(), currentItem, units);
			game::avatar::Items::Remove(currentItem, units);
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