#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.MouseMotion.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.h>
#include <Game.Avatar.Docked.h>
#include <Game.Avatar.Items.h>
#include <Game.Avatar.Ship.h>
#include <Game.Avatar.ShipStatistics.h>
#include <Game.Avatar.Statistics.h>
#include <Game.Colors.h>
#include <Game.Islands.Commodities.h>
#include <Game.Islands.Markets.h>
#include <Game.Islands.Ships.h>
#include <Game.Ships.h>
#include <Game.World.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.Confirmations.h>
#include <Visuals.Buttons.h>
#include <Visuals.Messages.h>
#include <Visuals.SpriteGrid.h>
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_SHIPYARD;
	static const std::string LAYOUT_NAME = "State.InPlay.Shipyard";
	static const std::string SPRITE_GRID_ID = "Grid";
	static const std::string FONT_DEFAULT = "default";

	static const std::string AREA_GO_BACK = "GoBack";
	static const std::string AREA_UNFOUL = "Unfoul";
	static const std::string AREA_LIST = "List";

	static const std::string BUTTON_GO_BACK = "GoBack";
	static const std::string BUTTON_UNFOUL = "Unfoul";

	static std::map<game::ShipType, double> shipPrices;
	static std::optional<size_t> hiliteRow = std::nullopt;

	static void UpdateShipPrices()
	{
		auto location = game::avatar::Docked::GetDockedLocation().value();
		auto prices = game::islands::Ships::GetPurchasePrices(location);
		auto tradeIn = game::islands::Ships::GetSalePrice(location, game::avatar::Ship::Read());
		shipPrices.clear();
		for (auto price : prices)
		{
			shipPrices[price.first] = price.second - tradeIn;
		}
	}

	static const auto WriteTextToGrid = visuals::SpriteGrid::DoWriteToGrid(LAYOUT_NAME, SPRITE_GRID_ID, FONT_DEFAULT, visuals::HorizontalAlignment::LEFT);

	static void OnLeave()
	{
		game::Avatar::DoAction(game::avatar::Action::ENTER_DOCK);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void RefreshStatistics()
	{
		WriteTextToGrid(
			{ 0, 19 },
			std::format(
				"Money: {:.3f}",
				game::avatar::Statistics::GetMoney()),
			game::Colors::WHITE);
		WriteTextToGrid(
			{ 0, 18 },
			std::format(
				"Fouling: {:.0f}%",
				game::avatar::ShipStatistics::GetFoulingPercentage()),
			game::Colors::WHITE);
	}

	static void RefreshShipPrices()
	{
		int row = 0;
		int gridRow = 2;
		auto money = game::avatar::Statistics::GetMoney();
		auto ship = game::avatar::Ship::Read();
		for (auto& shipPrice : shipPrices)
		{
			WriteTextToGrid(
				{ 0, gridRow },
				std::format("{:10s}{}| {:7.3f}",
					game::Ships::GetName(shipPrice.first),
					(ship == shipPrice.first) ? ("*") : (" "),
					shipPrice.second),
				(row == hiliteRow && money>= shipPrice.second) ? (game::Colors::CYAN) :
				(row == hiliteRow && money< shipPrice.second) ? (game::Colors::LIGHT_RED) :
				(money < shipPrice.second) ? (game::Colors::RED) :
				(game::Colors::GRAY));
			++gridRow;
			++row;
		}
	}

	static void RefreshGrid()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID_ID);
		WriteTextToGrid({ 0,1 }, "Ship Type    Price", game::Colors::YELLOW);
		RefreshShipPrices();
		RefreshStatistics();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		UpdateShipPrices();
		RefreshGrid();
	}

	static std::function<void()> DoBuyShip(game::ShipType desiredShip, double price)
	{
		return [desiredShip, price]()
		{
			auto location = game::avatar::Docked::GetDockedLocation().value();
			auto currentShip = game::avatar::Ship::Read();
			game::avatar::Statistics::ChangeMoney(-price);
			game::avatar::Ship::Write(desiredShip);
			game::islands::Markets::BuyShipType(location, desiredShip);
			game::islands::Markets::SellShipType(location, currentShip);
			UpdateShipPrices();
			RefreshGrid();
		};
	}

	static void CheckTonnage(game::ShipType desiredShip, double price)
	{
		if (game::avatar::Items::TotalTonnage() <= game::Ships::GetTotalTonnage(desiredShip))
		{
			visuals::Confirmations::Write(
				{
					"Are you sure?",
					DoBuyShip(desiredShip, price),
					[]() {}
				});
			application::UIState::Write(::UIState::IN_PLAY_NEXT);
		}
		else
		{
			visuals::Messages::Write({ "Too Much Cargo!",{} });
			application::UIState::Write(::UIState::IN_PLAY_NEXT);
		}
	}

	static void CheckAvailableFunds(game::ShipType desiredShip)
	{
		double price = shipPrices[desiredShip];
		if (game::avatar::Statistics::GetMoney() >= price)
		{
			CheckTonnage(desiredShip, price);
		}
		else
		{
			visuals::Messages::Write({ "Insufficient Funds!",{} });
			application::UIState::Write(::UIState::IN_PLAY_NEXT);
		}
	}

	static void TryBuyShip()
	{
		std::optional<game::ShipType> desiredShip = common::Utility::GetNthKey(shipPrices, hiliteRow);
		if (desiredShip)
		{
			CheckAvailableFunds(desiredShip.value());
		}
	}

	static const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, common::Utility::DoPreviousItem(hiliteRow, shipPrices, RefreshShipPrices) },
		{ ::Command::DOWN, common::Utility::DoNextItem(hiliteRow, shipPrices, RefreshShipPrices) },
		{ ::Command::GREEN, TryBuyShip },
		{ ::Command::BACK, OnLeave },
		{ ::Command::RED, OnLeave }
	};

	static std::function<void()> DoUnfoul(double price)
	{
		return [price]() 
		{
			game::avatar::Statistics::ChangeMoney(-price);
			game::avatar::ShipStatistics::CleanHull(game::Side::STARBOARD);
			game::avatar::ShipStatistics::CleanHull(game::Side::PORT);
		};
	}

	static void OnUnfoul()
	{
		double fouling = game::avatar::ShipStatistics::GetFouling();
		if (fouling > 0)
		{
			double labor = game::World::GetUnfoulingLaborMultiplier() * fouling;
			double price = game::islands::Commodities::GetPurchasePrice(game::avatar::Docked::GetDockedLocation().value(),
				{
					{game::Commodity::LABOR, labor}
				});
			if (game::avatar::Statistics::GetMoney() >= price)
			{
				visuals::Confirmations::Write(
					{
						"Are you sure?",
						DoUnfoul(price),
						[]() {}
					});
				application::UIState::Write(::UIState::IN_PLAY_NEXT);
			}
			else
			{
				visuals::Messages::Write({ "Insufficient Funds!",{} });
				application::UIState::Write(::UIState::IN_PLAY_NEXT);
			}
		}
	}

	static const std::map<std::string, std::function<void()>> buttonUpHandlers =
	{
		{AREA_GO_BACK, OnLeave},
		{AREA_UNFOUL, OnUnfoul},
		{AREA_LIST, TryBuyShip}
	};

	static bool OnMouseButtonUpInArea(const std::string& areaName)
	{
		return common::Utility::Dispatch(buttonUpHandlers, areaName, true, false);
	}

	static void OnMouseMotionGoBack(const common::XY<int>&)
	{
		visuals::Buttons::SetHoverButton(LAYOUT_NAME, BUTTON_GO_BACK);
	}

	static void OnMouseMotionUnfoul(const common::XY<int>&)
	{
		visuals::Buttons::SetHoverButton(LAYOUT_NAME, BUTTON_UNFOUL);
	}

	static void OnMouseMotionList(const common::XY<int>& xy)
	{
		size_t candidateRow = xy.GetY() / visuals::SpriteGrid::GetCellHeight(LAYOUT_NAME, SPRITE_GRID_ID);
		if (candidateRow < shipPrices.size())
		{
			hiliteRow = candidateRow;
		}
	}

	static const std::map<std::string, std::function<void(const common::XY<int>&)>> mouseMotionHandlers =
	{
		{AREA_GO_BACK, OnMouseMotionGoBack},
		{AREA_UNFOUL, OnMouseMotionUnfoul},
		{AREA_LIST, OnMouseMotionList}
	};

	static void OnMouseMotionInArea(const std::string& areaName, const common::XY<int>& position)
	{
		visuals::Buttons::ClearHoverButton(LAYOUT_NAME);
		hiliteRow = std::nullopt;
		common::Utility::DispatchParameter(mouseMotionHandlers, areaName, position);
		RefreshGrid();
	}

	static void OnMouseMotionOutsideAreas(const common::XY<int>&)
	{
		visuals::Buttons::ClearHoverButton(LAYOUT_NAME);
		hiliteRow = std::nullopt;
		RefreshGrid();
	}

	void Shipyard::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::MouseButtonUp::AddHandler(CURRENT_STATE, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::MouseMotion::AddHandler(CURRENT_STATE, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea, OnMouseMotionOutsideAreas));
		::application::Command::SetHandlers(CURRENT_STATE, commandHandlers);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}
