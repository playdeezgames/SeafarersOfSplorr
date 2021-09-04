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
#include <Game.Avatar.Statistics.h>
#include <Game.Colors.h>
#include <Game.Islands.Items.h>
#include <Game.Islands.Markets.h>
#include <Game.Items.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.Areas.h>
#include <Visuals.SpriteGrid.h>
#include <Visuals.Texts.h>
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_ISLAND_BUY;
	static const std::string LAYOUT_NAME = "State.InPlay.IslandBuy";
	static const std::string SPRITE_GRID_ID = "Grid";
	static const std::string FONT_DEFAULT = "default";
	static const std::string AREA_LIST = "List";
	static const std::string AREA_GO_BACK = "GoBack";
	static const std::string TEXT_GO_BACK = "GoBack";

	static const auto WriteTextToGrid = visuals::SpriteGrid::DoWriteToGrid(LAYOUT_NAME, SPRITE_GRID_ID, FONT_DEFAULT, visuals::HorizontalAlignment::LEFT);

	static void OnLeave()
	{
		game::Avatar::DoAction(game::avatar::Action::ENTER_MARKET);
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void RefreshHeader()
	{
		WriteTextToGrid({ 0, 0 }, "<-", game::Colors::YELLOW);
		WriteTextToGrid({ 37, 0 }, "->", game::Colors::YELLOW);
		WriteTextToGrid({ 15, 0 }, "Page 1 of 1", game::Colors::YELLOW);
		WriteTextToGrid({ 0,1 }, std::format("{:14s}   {:7s}   {:4s}   {:5s}", "Item", "  Price", " Own", "Tonn."), game::Colors::YELLOW);
	}

	static std::map<game::Item, double> unitPrices;
	static size_t hiliteRow = 0;
	static bool hoverGoBack = false;

	static void UpdateUnitPrices()
	{
		unitPrices = game::islands::Items::GetPurchasePrices(game::avatar::Docked::GetDockedLocation().value());
	}

	static void RefreshUnitPrices()
	{
		int row = 0;
		int gridRow = 2;
		for (auto& unitPrice : unitPrices)
		{
			auto& itemDescriptor = game::Items::Read(unitPrice.first);
			WriteTextToGrid(
				{ 0, gridRow }, 
				std::format("{:14s} | {:7.3f} | {:4d} | {:5.3f}", 
					itemDescriptor.name, 
					unitPrice.second,
					game::avatar::Items::Read(unitPrice.first),
					itemDescriptor.tonnage), 
				(row==hiliteRow) ? (game::Colors::CYAN) : (game::Colors::GRAY));
			++gridRow;
			++row;
		}
	}

	static void RefreshStatistics()
	{
		WriteTextToGrid(
			{ 0, 18 },
			std::format(
				"Avail. Tonn.: {:.3f}",
				game::avatar::Ship::AvailableTonnage()),
			game::Colors::WHITE);
		WriteTextToGrid(
			{ 0, 19 }, 
			std::format(
				"Money: {:.3f}",
				game::avatar::Statistics::GetMoney()),
			game::Colors::WHITE);
	}

	static void RefreshButton()
	{
		visuals::Texts::SetColor(LAYOUT_NAME, TEXT_GO_BACK, (hoverGoBack) ? (game::Colors::CYAN) : (game::Colors::GRAY));
	}

	static void RefreshGrid()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID_ID);
		RefreshHeader();
		RefreshUnitPrices();
		RefreshStatistics();
	}

	static void Refresh()
	{
		RefreshButton();
		RefreshGrid();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		UpdateUnitPrices();
		Refresh();
	}

	static void BuyItem()
	{
		auto item = common::Utility::GetNthKey(unitPrices, hiliteRow);
		if (item)
		{
			double price = unitPrices[item.value()];
			if (game::avatar::Statistics::GetMoney() >= price)
			{
				if (game::avatar::Ship::AvailableTonnage() >= game::Items::Read(item.value()).tonnage)
				{
					game::avatar::Statistics::ChangeMoney(-price);
					game::islands::Markets::BuyItems(game::avatar::Docked::GetDockedLocation().value(), item.value(), 1);
					game::avatar::Items::Add(item.value(), 1);

					UpdateUnitPrices();
					Refresh();
				}
			}
		}
	}

	static bool OnMouseButtonUpInArea(const std::string& areaName)
	{
		if (areaName == AREA_GO_BACK)
		{
			OnLeave();
		}
		else if (areaName == AREA_LIST)
		{
			BuyItem();
		}
		return true;
	}

	static const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, common::Utility::DoPreviousItem(hiliteRow, unitPrices, Refresh) },
		{ ::Command::DOWN, common::Utility::DoNextItem(hiliteRow, unitPrices, Refresh) },
		{ ::Command::GREEN, BuyItem },
		{ ::Command::BACK, OnLeave },
		{ ::Command::RED, OnLeave }
	};

	static void OnMouseMotionInArea(const std::string& areaName, const common::XY<int>& location)
	{
		hoverGoBack = false;
		if (areaName == AREA_LIST)
		{
			auto cellHeight = visuals::SpriteGrid::GetCellHeight(LAYOUT_NAME, SPRITE_GRID_ID);
			hiliteRow = location.GetY() / cellHeight;
		}
		else if (areaName == AREA_GO_BACK)
		{
			hoverGoBack = true;
		}
		Refresh();
	}

	static void OnMouseMotionOutsideArea(const common::XY<int>&)
	{
		hoverGoBack = false;
		Refresh();
	}

	void IslandBuy::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::MouseButtonUp::AddHandler(CURRENT_STATE, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::MouseMotion::AddHandler(CURRENT_STATE, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea, OnMouseMotionOutsideArea));
		::application::Command::SetHandlers(CURRENT_STATE, commandHandlers);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}
