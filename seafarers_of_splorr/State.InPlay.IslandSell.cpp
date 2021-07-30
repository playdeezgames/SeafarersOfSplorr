#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.OnEnter.h"
#include "Application.Renderer.h"
#include "Application.UIState.h"
#include <format>
#include "Game.Audio.Mux.h"
#include "Game.Avatar.h"
#include "Game.Avatar.Items.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Islands.Items.h"
#include "Game.Islands.Markets.h"
#include "Game.Items.h"
#include "Visuals.Data.Colors.h"
#include "Visuals.SpriteGrid.h"
namespace state::in_play::IslandSell
{
	const std::string LAYOUT_NAME = "State.InPlay.IslandSell";
	const std::string SPRITE_GRID_ID = "Grid";
	const std::string FONT_DEFAULT = "default";

	static void WriteTextToGrid(const common::XY<int> location, const std::string& text, const std::string& color)
	{
		visuals::SpriteGrid::WriteText(LAYOUT_NAME, SPRITE_GRID_ID, location, FONT_DEFAULT, text, color);
	}

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
		WriteTextToGrid({ 0,1 }, std::format("{:15s}   {:7s}   {:4s}", "Item", "  Price", " Own"), visuals::data::Colors::HIGHLIGHT);
	}

	static std::map<game::Item, double> unitPrices;
	static int hiliteRow = 0;

	static void UpdateUnitPrices()
	{
		unitPrices = game::islands::Items::GetSalePrices(game::Avatar::GetDockedLocation().value());
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
				std::format("{:15s} | {:7.3f} | {:4d}",
					itemDescriptor.name,
					unitPrice.second,
					game::avatar::Items::Read(unitPrice.first)),
				(row == hiliteRow) ? (visuals::data::Colors::HOVER) : (visuals::data::Colors::NORMAL));
			++gridRow;
			++row;
		}
	}

	static double GetMoney()
	{
		return game::avatar::Statistics::GetCurrent(game::avatar::Statistic::MONEY);
	}

	static void RefreshStatistics()
	{
		WriteTextToGrid(
			{ 0, 19 },
			std::format(
				"Money: {:.3f}",
				GetMoney()),
			visuals::data::Colors::NORMAL);
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

	static void PreviousItem()
	{
		hiliteRow = (hiliteRow + (int)unitPrices.size() - 1) % (int)unitPrices.size();
		RefreshUnitPrices();
	}

	static void NextItem()
	{
		hiliteRow = (hiliteRow + 1) % (int)unitPrices.size();
		RefreshUnitPrices();
	}

	static void SellItem()
	{
		auto unitPrice = unitPrices.begin();
		int index = hiliteRow;
		while (index > 0)
		{
			unitPrice++;
			index--;
		}
		auto owned = game::avatar::Items::Read(unitPrice->first);
		if (owned>0)
		{
			game::avatar::Statistics::ChangeCurrent(game::avatar::Statistic::MONEY, +unitPrice->second);
			game::islands::Markets::SellItems(game::Avatar::GetDockedLocation().value(), unitPrice->first, 1);
			game::avatar::Items::Remove(unitPrice->first, 1);
			UpdateUnitPrices();
			RefreshGrid();
		}
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, PreviousItem },
		{ ::Command::DOWN, NextItem },
		{ ::Command::GREEN, SellItem },
		{ ::Command::BACK, ::application::UIState::GoTo(::UIState::IN_PLAY_ISLAND_TRADE) },
		{ ::Command::RED, ::application::UIState::GoTo(::UIState::IN_PLAY_ISLAND_TRADE) }
	};

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_ISLAND_SELL, OnEnter);
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_ISLAND_SELL, OnMouseButtonUp);
		::application::Command::SetHandlers(::UIState::IN_PLAY_ISLAND_SELL, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_ISLAND_SELL, LAYOUT_NAME);
	}
}
