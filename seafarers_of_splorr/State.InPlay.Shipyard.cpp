#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.OnEnter.h"
#include "Application.Renderer.h"
#include "Application.UIState.h"
#include "Common.Utility.h"
#include <format>
#include "Game.Audio.Mux.h"
#include "Game.Avatar.h"
#include "Game.Avatar.Ship.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Islands.Markets.h"
#include "Game.Islands.Ships.h"
#include "Game.Ships.h"
#include "Visuals.Data.Colors.h"
#include "Visuals.SpriteGrid.h"
namespace state::in_play::Shipyard
{
	const std::string LAYOUT_NAME = "State.InPlay.Shipyard";
	const std::string SPRITE_GRID_ID = "Grid";
	const std::string FONT_DEFAULT = "default";

	static std::map<game::Ship, double> shipPrices;
	static size_t hiliteRow = 0;

	static void UpdateShipPrices()
	{
		shipPrices = game::islands::Ships::GetPurchasePrices(game::Avatar::GetDockedLocation().value());
	}

	const auto WriteTextToGrid = visuals::SpriteGrid::DoWriteToGrid(LAYOUT_NAME, SPRITE_GRID_ID, FONT_DEFAULT, visuals::HorizontalAlignment::LEFT);

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		::application::UIState::Write(::UIState::IN_PLAY_DOCKED);
		return true;
	}

	static void RefreshStatistics()
	{
		WriteTextToGrid(
			{ 0, 19 },
			std::format(
				"Money: {:.3f}",
				game::avatar::Statistics::GetMoney()),
			visuals::data::Colors::DEFAULT);
	}

	static void RefreshShipPrices()
	{
		int row = 0;
		int gridRow = 2;
		for (auto& unitPrice : shipPrices)
		{
			auto& shipDescriptor = game::Ships::Read(unitPrice.first);
			WriteTextToGrid(
				{ 0, gridRow },
				std::format("{:15s} | {:7.3f}",
					shipDescriptor.name,
					unitPrice.second),
				(row == hiliteRow) ? (visuals::data::Colors::HOVER) : (visuals::data::Colors::NORMAL));
			++gridRow;
			++row;
		}
	}

	static void RefreshGrid()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID_ID);
		RefreshShipPrices();
		RefreshStatistics();
	}

	void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		UpdateShipPrices();
		RefreshGrid();
	}

	static void BuyShip()
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
		{ ::Command::UP, common::Utility::DoPreviousItem(hiliteRow, shipPrices, RefreshShipPrices) },
		{ ::Command::DOWN, common::Utility::DoNextItem(hiliteRow, shipPrices, RefreshShipPrices) },
		{ ::Command::GREEN, BuyShip },
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
