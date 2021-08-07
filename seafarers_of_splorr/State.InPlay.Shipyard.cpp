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
namespace state::in_play::Shipyard
{
	const std::string LAYOUT_NAME = "State.InPlay.Shipyard";
	const std::string SPRITE_GRID_ID = "Grid";
	const std::string FONT_DEFAULT = "default";

	static void WriteTextToGrid(const common::XY<int> location, const std::string& text, const std::string& color)
	{
		visuals::SpriteGrid::WriteText(LAYOUT_NAME, SPRITE_GRID_ID, location, FONT_DEFAULT, text, color, visuals::HorizontalAlignment::LEFT);
	}

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		::application::UIState::Write(::UIState::IN_PLAY_DOCKED);
		return true;
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
			visuals::data::Colors::DEFAULT);
	}

	static void RefreshGrid()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID_ID);
		RefreshStatistics();
	}

	void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		RefreshGrid();
	}

	static void PreviousItem()
	{
		//hiliteRow = (hiliteRow + (int)unitPrices.size() - 1) % (int)unitPrices.size();
		//RefreshUnitPrices();
	}

	static void NextItem()
	{
		//hiliteRow = (hiliteRow + 1) % (int)unitPrices.size();
		//RefreshUnitPrices();
	}

	static void BuyItem()
	{
		//auto unitPrice = unitPrices.begin();
		//int index = hiliteRow;
		//while (index > 0)
		//{
		//	unitPrice++;
		//	index--;
		//}
		//if (GetMoney() >= unitPrice->second)
		//{
		//	if (game::avatar::Ship::AvailableTonnage() >= game::Items::Read(unitPrice->first).tonnage)
		//	{
		//		game::avatar::Statistics::ChangeCurrent(game::avatar::Statistic::MONEY, -unitPrice->second);
		//		game::islands::Markets::BuyItems(game::Avatar::GetDockedLocation().value(), unitPrice->first, 1);
		//		game::avatar::Items::Add(unitPrice->first, 1);
		//		UpdateUnitPrices();
		//		RefreshGrid();
		//	}
		//}
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, PreviousItem },
		{ ::Command::DOWN, NextItem },
		{ ::Command::GREEN, BuyItem },
		{ ::Command::BACK, ::application::UIState::GoTo(::UIState::IN_PLAY_DOCKED) },
		{ ::Command::RED, ::application::UIState::GoTo(::UIState::IN_PLAY_DOCKED) }
	};

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_SHIPYARD, OnEnter);
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_SHIPYARD, OnMouseButtonUp);
		::application::Command::SetHandlers(::UIState::IN_PLAY_SHIPYARD, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_SHIPYARD, LAYOUT_NAME);
	}
}
