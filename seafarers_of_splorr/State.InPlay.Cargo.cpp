#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.OnEnter.h"
#include "Application.Renderer.h"
#include <format>
#include "Game.Audio.Mux.h"
#include "Game.Avatar.h"
#include "Game.Avatar.Items.h"
#include "Game.Avatar.Ship.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Islands.Items.h"
#include "Game.Islands.Markets.h"
#include "Game.Items.h"
#include "Visuals.SpriteGrid.h"
namespace state::in_play::Cargo
{
	const std::string LAYOUT_NAME = "State.InPlay.Cargo";
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
		::application::UIState::Write(::UIState::IN_PLAY_AT_SEA);
		return true;
	}

	static void RefreshHeader()
	{
		WriteTextToGrid({ 0, 0 }, "<-", COLOR_PAGE_HEADER);
		WriteTextToGrid({ 37, 0 }, "->", COLOR_PAGE_HEADER);
		WriteTextToGrid({ 15, 0 }, "Page 1 of 1", COLOR_PAGE_HEADER);
		WriteTextToGrid({ 0,1 }, std::format("{:15s}   {:4s}", "Item", "  Price", " Own"), COLOR_TABLE_HEADER);
	}

	static std::map<game::Item, size_t> manifest;
	static int hiliteRow = 0;

	static void UpdateManifest()
	{
		manifest = game::avatar::Items::All();
	}

	static void RefreshManifest()
	{
		int row = 0;
		int gridRow = 2;
		for (auto& entry : manifest)
		{
			auto& itemDescriptor = game::Items::Read(entry.first);
			WriteTextToGrid(
				{ 0, gridRow },
				std::format("{:15s} | {:4d}",
					itemDescriptor.name,
					entry.second),
				(row == hiliteRow) ? (COLOR_HILITE) : (COLOR_DEFAULT));
			++gridRow;
			++row;
		}
	}

	static double GetMoney()
	{
		return game::avatar::Statistics::GetCurrent(game::avatar::Statistic::MONEY);
	}

	static double GetTonnage()
	{
		return game::avatar::Items::TotalTonnage();
	}

	static double GetAvailableTonnage()
	{
		return game::avatar::Ship::AvailableTonnage();
	}

	static void RefreshStatistics()
	{
		WriteTextToGrid(
			{ 0, 18 },
			std::format(
				"Tonnage: {:.3f} ({:d}%)",
				GetTonnage(),
				(int)(100.0 * GetTonnage() / GetAvailableTonnage())),
			COLOR_DEFAULT);
		WriteTextToGrid(
			{ 0, 19 },
			std::format(
				"Money: {:.3f}",
				GetMoney()),
			COLOR_DEFAULT);
	}

	static void RefreshGrid()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID_ID);
		RefreshHeader();
		RefreshManifest();
		RefreshStatistics();
	}

	void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		UpdateManifest();
		RefreshGrid();
	}

	static void PreviousItem()
	{
		hiliteRow = (hiliteRow + (int)manifest.size() - 1) % (int)manifest.size();
		RefreshManifest();
	}

	static void NextItem()
	{
		hiliteRow = (hiliteRow + 1) % (int)manifest.size();
		RefreshManifest();
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, PreviousItem },
		{ ::Command::DOWN, NextItem },
		{ ::Command::BACK, ::application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA) },
		{ ::Command::RED, ::application::UIState::GoTo(::UIState::IN_PLAY_AT_SEA) }
	};

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_CARGO, OnEnter);
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_CARGO, OnMouseButtonUp);
		::application::Command::SetHandlers(::UIState::IN_PLAY_CARGO, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_CARGO, LAYOUT_NAME);
	}
}
