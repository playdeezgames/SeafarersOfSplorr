#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.OnEnter.h"
#include "Application.Renderer.h"
#include "Application.UIState.h"
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
namespace state::in_play::Cargo
{
	const std::string LAYOUT_NAME = "State.InPlay.Cargo";
	const std::string SPRITE_GRID_ID = "Grid";
	const std::string FONT_DEFAULT = "default";

	static void WriteTextToGrid(const common::XY<int> location, const std::string& text, const std::string& color)
	{
		visuals::SpriteGrid::WriteText(LAYOUT_NAME, SPRITE_GRID_ID, location, FONT_DEFAULT, text, color);
	}

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		::application::UIState::Write(::UIState::IN_PLAY_AT_SEA);
		return true;
	}

	static void RefreshHeader()
	{
		WriteTextToGrid({ 0, 0 }, "<-", visuals::data::Colors::HIGHLIGHT);
		WriteTextToGrid({ 37, 0 }, "->", visuals::data::Colors::HIGHLIGHT);
		WriteTextToGrid({ 15, 0 }, "Page 1 of 1", visuals::data::Colors::HIGHLIGHT);
		WriteTextToGrid({ 0,1 }, std::format("{:15s}   {:4s}", "Item", "  Price", " Own"), visuals::data::Colors::HIGHLIGHT);
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
				(row == hiliteRow) ? (visuals::data::Colors::HOVER) : (visuals::data::Colors::NORMAL));
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
			visuals::data::Colors::NORMAL);
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