#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.OnEnter.h"
#include "Application.Renderer.h"
#include "Application.UIState.h"
#include "Common.Utility.h"
#include <format>
#include "Game.Audio.Mux.h"
#include "Game.Avatar.Items.h"
#include "Game.Avatar.Ship.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Islands.Items.h"
#include "Game.Islands.Markets.h"
#include "Game.Items.h"
#include "UIState.h"
#include "Game.Colors.h"
#include "Visuals.SpriteGrid.h"
namespace state::in_play::Cargo
{
	const std::string LAYOUT_NAME = "State.InPlay.Cargo";
	const std::string SPRITE_GRID_ID = "Grid";
	const std::string FONT_DEFAULT = "default";

	const auto WriteTextToGrid = visuals::SpriteGrid::DoWriteToGrid(LAYOUT_NAME, SPRITE_GRID_ID, FONT_DEFAULT, visuals::HorizontalAlignment::LEFT);

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		::application::UIState::Write(::UIState::IN_PLAY_AT_SEA);
		return true;
	}

	static void RefreshHeader()
	{
		WriteTextToGrid({ 0, 0 }, "<-", game::Colors::YELLOW);
		WriteTextToGrid({ 37, 0 }, "->", game::Colors::YELLOW);
		WriteTextToGrid({ 15, 0 }, "Page 1 of 1", game::Colors::YELLOW);
		WriteTextToGrid({ 0,1 }, std::format("{:15s}   {:4s}", "Item", "  Price", " Own"), game::Colors::YELLOW);
	}

	static std::map<game::Item, size_t> manifest;
	static size_t hiliteRow = 0;

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
				(row == hiliteRow) ? (game::Colors::CYAN) : (game::Colors::GRAY));
			++gridRow;
			++row;
		}
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
			game::Colors::GRAY);
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
		RefreshManifest();
		RefreshStatistics();
	}

	void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		UpdateManifest();
		RefreshGrid();
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, common::Utility::DoPreviousItem(hiliteRow, manifest, RefreshManifest) },
		{ ::Command::DOWN, common::Utility::DoNextItem(hiliteRow, manifest, RefreshManifest) },
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