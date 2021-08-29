#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <format>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.h>
#include <Game.Avatar.Docked.h>
#include <Game.Avatar.Items.h>
#include <Game.Avatar.Statistics.h>
#include <Game.Colors.h>
#include <Game.Demigods.h>
#include <Game.Islands.h>
#include <Game.Islands.Items.h>
#include <Game.Islands.Markets.h>
#include <Game.Items.h>
#include "UIState.h"
#include <Visuals.SpriteGrid.h>
namespace state::in_play::MakeOffering
{
	const ::UIState CURRENT_LAYOUT = ::UIState::IN_PLAY_MAKE_OFFERING;
	const std::string LAYOUT_NAME = "State.InPlay.MakeOffering";
	const std::string SPRITE_GRID_ID = "Grid";
	const std::string FONT_DEFAULT = "default";

	const auto WriteTextToGrid = visuals::SpriteGrid::DoWriteToGrid(LAYOUT_NAME, SPRITE_GRID_ID, FONT_DEFAULT, visuals::HorizontalAlignment::LEFT);

	static void OnLeave()
	{
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		OnLeave();
		return true;
	}

	static void RefreshHeader()
	{
		WriteTextToGrid({ 0, 0 }, "<-", game::Colors::YELLOW);
		WriteTextToGrid({ 37, 0 }, "->", game::Colors::YELLOW);
		WriteTextToGrid({ 15, 0 }, "Page 1 of 1", game::Colors::YELLOW);
		WriteTextToGrid({ 0,1 }, std::format("{:15s}", "Item"), game::Colors::YELLOW);
	}

	static std::map<game::Item, size_t> items;
	static size_t hiliteRow = 0;

	static void UpdateItems()
	{
		items = game::avatar::Items::All();
	}

	static void RefreshItems()
	{
		int row = 0;
		int gridRow = 2;
		for (auto& item : items)
		{
			auto& itemDescriptor = game::Items::Read(item.first);
			WriteTextToGrid(
				{ 0, gridRow },
				std::format("{:15s}",
					itemDescriptor.name),
				(row == hiliteRow) ? (game::Colors::CYAN) : (game::Colors::GRAY));
			++gridRow;
			++row;
		}
	}

	static void RefreshStatistics()
	{
		WriteTextToGrid(
			{ 0, 19 },
			std::format(
				"Money: {:.3f}",
				game::avatar::Statistics::GetMoney()),
			game::Colors::GRAY);
	}

	static void RefreshGrid()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID_ID);
		RefreshHeader();
		RefreshItems();
		RefreshStatistics();
	}

	void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		UpdateItems();
		RefreshGrid();
	}

	static void OfferItem()
	{
		auto item = common::Utility::GetNthKey(items, hiliteRow);
		if (item)
		{
			//TODO: message about offering
			auto location = game::avatar::Docked::GetDockedLocation().value();
			auto island = game::Islands::Read(location).value();
			game::avatar::Items::Remove(item.value(), 1);
			game::Demigods::MakeOffering(island.patronDemigod, item.value());
			UpdateItems();
			RefreshGrid();
		}
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, common::Utility::DoPreviousItem(hiliteRow, items, RefreshItems) },
		{ ::Command::DOWN, common::Utility::DoNextItem(hiliteRow, items, RefreshItems) },
		{ ::Command::GREEN, OfferItem },
		{ ::Command::BACK, OnLeave },
		{ ::Command::RED, OnLeave }
	};

	void Start()
	{
		::application::OnEnter::AddHandler(CURRENT_LAYOUT, OnEnter);
		::application::MouseButtonUp::AddHandler(CURRENT_LAYOUT, OnMouseButtonUp);
		::application::Command::SetHandlers(CURRENT_LAYOUT, commandHandlers);
		::application::Renderer::SetRenderLayout(CURRENT_LAYOUT, LAYOUT_NAME);
	}
}
