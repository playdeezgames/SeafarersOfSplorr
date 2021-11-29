#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Data.h>
#include <Common.Utility.Table.h>
#include <Data.Game.Island.h>
#include <Game.Audio.Mux.h>
#include <Game.Character.Docked.h>
#include <Game.Character.Items.h>
#include <Game.Character.Ship.h>
#include <Game.Character.Statistics.h>
#include <Game.Colors.h>
#include <Game.Islands.Markets.h>
#include <Game.Islands.Ships.h>
#include <Game.Player.h>
#include <Game.Ship.h>
#include <Game.ShipNames.h>
#include <Game.ShipTypes.h>
#include "State.InPlay.ShipyardPurchaseShip.h"
#include "State.Terminal.h"
#include "UIState.h"
#include <Visuals.Confirmations.h>
#include <Visuals.Messages.h>
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_SHIPYARD_PURCHASE_SHIP;

	static std::map<game::ShipType, double> shipPrices;

	static void UpdateShipPrices()
	{
		auto location =
			data::game::Island::Read(
				game::character::Docked::ReadLocation(game::Player::GetCharacterId()).value()
			).value().location;
		auto prices = game::islands::Ships::GetPurchasePrices(game::character::Docked::ReadLocation(game::Player::GetCharacterId()).value());
		auto tradeIn = game::islands::Ships::GetSalePrice(game::character::Docked::ReadLocation(game::Player::GetCharacterId()).value(), game::Ship::GetShipType(game::character::Ship::ReadShipId(game::Player::GetCharacterId()).value()).value());
		shipPrices.clear();
		for (auto price : prices)
		{
			shipPrices[price.first] = price.second - tradeIn;
		}
	}

	static void RefreshShipPrices()
	{
		size_t index = 1;
		Terminal::SetForeground(game::Colors::YELLOW);
		for (auto shipPrice : shipPrices)
		{
			Terminal::WriteLine(
				"{}) {} (net cost {:.4f})", 
				index++, 
				game::ShipTypes::GetName(shipPrice.first), 
				shipPrice.second);
		}
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Purchase Ship:");
		int shipId = game::character::Ship::ReadShipId(game::Player::GetCharacterId()).value();
		auto shipType = game::Ship::GetShipType(shipId).value();
		Terminal::SetForeground(game::Colors::GRAY);
		Terminal::WriteLine("You currently have a {}.", game::ShipTypes::GetName(shipType));
		Terminal::WriteLine("You have {:.4f}.", game::character::Statistics::ReadMoney(game::Player::GetCharacterId()));
		Terminal::WriteLine("Prices shown after trade-in value.");
		RefreshShipPrices();

		Terminal::SetForeground(game::Colors::YELLOW);
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();

	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		UpdateShipPrices();
		Refresh();
	}

	static void OnLeave()
	{
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "0", OnLeave }
	};

	static void BuyShip(game::ShipType desiredShipType, double price)
	{
		auto location =
			data::game::Island::Read(
				game::character::Docked::ReadLocation(game::Player::GetCharacterId()).value()
			).value().location;

		auto currentShipId = game::character::Ship::ReadShipId(game::Player::GetCharacterId()).value();
		auto currentShipType = game::Ship::GetShipType(currentShipId).value();
		game::character::Statistics::ChangeMoney(game::Player::GetCharacterId(), -price);
		auto desiredShipId = game::Ship::Add({ desiredShipType,game::ShipNames::Generate(), location, 0.0, 1.0 });
		//TODO: transfer crew/passengers/captives?
		game::character::Ship::Write(game::Player::GetCharacterId() , desiredShipId, game::BerthType::CAPTAIN);
		game::islands::Markets::BuyShipType(location, desiredShipType);
		game::islands::Markets::SellShipType(location, currentShipType);
		UpdateShipPrices();
		Refresh();
	}

	static void CheckTonnage(game::ShipType desiredShip, double price)
	{
		if (game::character::Items::TotalTonnage(game::Player::GetCharacterId()) <= game::ShipTypes::GetTotalTonnage(desiredShip))
		{
			Terminal::WriteLine();
			Terminal::SetForeground(game::Colors::GREEN);
			Terminal::WriteLine("You buy a new ship!");
			BuyShip(desiredShip, price);
			Refresh();
		}
		else
		{
			Terminal::ErrorMessage("Too Much Cargo!");
			Refresh();
		}
	}

	static void CheckAvailableFunds(game::ShipType desiredShip)
	{
		double price = shipPrices[desiredShip];
		if (game::character::Statistics::ReadMoney(game::Player::GetCharacterId()) >= price)
		{
			CheckTonnage(desiredShip, price);
		}
		else
		{
			Terminal::ErrorMessage("Insufficient Funds!");
			Refresh();
		}
	}

	static void TryBuyShip(int hiliteRow)
	{
		game::ShipType desiredShip = common::utility::Table::GetNthKey(shipPrices, hiliteRow).value();
		CheckAvailableFunds(desiredShip);
	}

	static void OnOtherInput(const std::string& line)
	{
		int index = common::Data::ToInt(line) - 1;
		if (index >= 0 && index < shipPrices.size())
		{
			TryBuyShip(index);
		}
		else
		{
			Terminal::ErrorMessage(Terminal::INVALID_INPUT);
			Refresh();
		}
	}

	void ShipyardPurchaseShip::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,OnOtherInput));
	}
}