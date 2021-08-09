#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.OnEnter.h"
#include "Application.Renderer.h"
#include "Application.UIState.h"
#include "Common.Utility.h"
#include <format>
#include "Game.Audio.Mux.h"
#include "Game.Avatar.h"
#include "Game.Avatar.Items.h"
#include "Game.Avatar.Ship.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Islands.Items.h"
#include "Game.Islands.Markets.h"
#include "Game.Items.h"
#include "Visuals.Data.Colors.h"
#include "Visuals.SpriteGrid.h"
namespace state::in_play::IslandBuy
{
	const std::string LAYOUT_NAME = "State.InPlay.IslandBuy";
	const std::string SPRITE_GRID_ID = "Grid";
	const std::string FONT_DEFAULT = "default";

	const auto WriteTextToGrid = visuals::SpriteGrid::DoWriteToGrid(LAYOUT_NAME, SPRITE_GRID_ID, FONT_DEFAULT, visuals::HorizontalAlignment::LEFT);

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		::application::UIState::Write(::UIState::IN_PLAY_ISLAND_TRADE);
		return true;
	}

	static void RefreshHeader()
	{
		WriteTextToGrid({ 0, 0 }, "<-", visuals::data::Colors::HIGHLIGHT);
		WriteTextToGrid({ 37, 0 }, "->", visuals::data::Colors::HIGHLIGHT);
		WriteTextToGrid({ 15, 0 }, "Page 1 of 1", visuals::data::Colors::HIGHLIGHT);
		WriteTextToGrid({ 0,1 }, std::format("{:15s}   {:7s}   {:4s}   {:5s}", "Item", "  Price", " Own", "Tonn."), visuals::data::Colors::HIGHLIGHT);
	}

	static std::map<game::Item, double> unitPrices;
	static size_t hiliteRow = 0;

	static void UpdateUnitPrices()
	{
		unitPrices = game::islands::Items::GetPurchasePrices(game::Avatar::GetDockedLocation().value());
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
				std::format("{:15s} | {:7.3f} | {:4d} | {:5.3f}", 
					itemDescriptor.name, 
					unitPrice.second,
					game::avatar::Items::Read(unitPrice.first),
					itemDescriptor.tonnage), 
				(row==hiliteRow) ? (visuals::data::Colors::HOVER) : (visuals::data::Colors::NORMAL));
			++gridRow;
			++row;
		}
	}

	static void RefreshStatistics()
	{
		WriteTextToGrid(
			{ 0, 18 },
			std::format(
				"Available Tonnage: {:.3f}",
				game::avatar::Ship::AvailableTonnage()),
			visuals::data::Colors::DEFAULT);
		WriteTextToGrid(
			{ 0, 19 }, 
			std::format(
				"Money: {:.3f}",
				game::avatar::Statistics::GetMoney()),
			visuals::data::Colors::DEFAULT);
	}

	static void RefreshGrid()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID_ID);
		RefreshHeader();
		RefreshUnitPrices();
		RefreshStatistics();
	}

	void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		UpdateUnitPrices();
		RefreshGrid();
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
					game::islands::Markets::BuyItems(game::Avatar::GetDockedLocation().value(), item.value(), 1);
					game::avatar::Items::Add(item.value(), 1);

					UpdateUnitPrices();
					RefreshGrid();
				}
			}
		}
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, common::Utility::DoPreviousItem(hiliteRow, unitPrices, RefreshUnitPrices) },
		{ ::Command::DOWN, common::Utility::DoNextItem(hiliteRow, unitPrices, RefreshUnitPrices) },
		{ ::Command::GREEN, BuyItem },
		{ ::Command::BACK, ::application::UIState::GoTo(::UIState::IN_PLAY_ISLAND_TRADE) },
		{ ::Command::RED, ::application::UIState::GoTo(::UIState::IN_PLAY_ISLAND_TRADE) }
	};

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_ISLAND_BUY, OnEnter);
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_ISLAND_BUY, OnMouseButtonUp);
		::application::Command::SetHandlers(::UIState::IN_PLAY_ISLAND_BUY, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_ISLAND_BUY, LAYOUT_NAME);
	}
}
