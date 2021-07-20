#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.OnEnter.h"
#include "Game.Audio.Mux.h"
#include "Visuals.SpriteGrid.h"
#include "Game.Items.h"
#include "Game.Islands.Items.h"
#include "Game.Avatar.h"
#include <format>
#include "Game.Avatar.Items.h"
namespace state::in_play::IslandBuy
{
	const std::string LAYOUT_NAME = "State.InPlay.IslandBuy";
	const std::string SPRITE_GRID_ID = "Grid";
	const std::string FONT_DEFAULT = "default";
	const std::string COLOR_DEFAULT = "Gray";
	const std::string COLOR_HILITE = "Cyan";
	const std::string COLOR_PAGE_HEADER = "Yellow";
	const std::string COLOR_TABLE_HEADER = "Yellow";

	static void WriteTextToGrid(const common::XY<int> location, const std::string& text, const std::string& color)
	{
		visuals::SpriteGrid::WriteText(LAYOUT_NAME, SPRITE_GRID_ID, location, FONT_DEFAULT, text, color);
	}

	static bool OnMouseButtonUp(const common::XY<int>& xy, unsigned char buttons)
	{
		::application::UIState::Write(::UIState::IN_PLAY_ISLAND_TRADE);
		return true;
	}

	static void RefreshHeader()
	{
		WriteTextToGrid({ 0, 0 }, "<-", COLOR_PAGE_HEADER);
		WriteTextToGrid({ 37, 0 }, "->", COLOR_PAGE_HEADER);
		WriteTextToGrid({ 15, 0 }, "Page 1 of 1", COLOR_PAGE_HEADER);
		WriteTextToGrid({ 0,1 }, std::format("{:15s}   {:7s}   {:4s}", "Item", "  Price", " Own"), COLOR_TABLE_HEADER);
	}

	static std::map<game::Item, double> unitPrices;
	static int hiliteRow = 0;

	static void UpdateUnitPrices()
	{
		unitPrices = game::islands::Items::GetPrices(game::Avatar::GetDockedLocation().value());
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
				(row==hiliteRow) ? (COLOR_HILITE) : (COLOR_DEFAULT));
			++gridRow;
			++row;
		}
	}

	void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		UpdateUnitPrices();
		RefreshHeader();
		RefreshUnitPrices();
	}

	static void PreviousItem()
	{
		hiliteRow = (hiliteRow + unitPrices.size() - 1) % unitPrices.size();
		RefreshUnitPrices();
	}

	static void NextItem()
	{
		hiliteRow = (hiliteRow + 1) % unitPrices.size();
		RefreshUnitPrices();
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, PreviousItem },
		{ ::Command::DOWN, NextItem },
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
