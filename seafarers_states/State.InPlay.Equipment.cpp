#include <Application.Renderer.h>
#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.OnEnter.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.Equipment.h>
#include <Game.Colors.h>
#include <Game.EquipSlots.h>
#include <Game.Items.h>
#include "States.h"
#include "UIState.h"
#include <Visuals.SpriteGrid.h>
namespace state::in_play
{
	static const std::string LAYOUT_NAME = "State.InPlay.Equipment";
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_EQUIPMENT;
	static const std::string SPRITE_GRID_ID = "Grid";
	static const std::string FONT_DEFAULT = "default";
	static const std::string EMPTY = "(empty)";

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		::application::UIState::Pop();
		return true;
	}

	static const std::list<game::EquipSlot> equipSlots =
	{
		game::EquipSlot::LEGS,
		game::EquipSlot::PRISON_WALLET
	};
	static size_t equipSlotIndex = 0;

	static void RefreshEquipSlot(int& row, size_t index, const game::EquipSlot& equipSlot)
	{
		auto color = (index == equipSlotIndex) ? (game::Colors::WHITE) : (game::Colors::DARK_GRAY);
		auto equipSlotDescriptor = game::EquipSlots::Read(equipSlot);
		visuals::SpriteGrid::WriteText(LAYOUT_NAME, SPRITE_GRID_ID, { 0, row }, FONT_DEFAULT, equipSlotDescriptor.name, color, visuals::HorizontalAlignment::LEFT);
		++row;

		auto item = game::avatar::Equipment::Read(equipSlot);
		if (item)
		{
			auto itemDescriptor = game::Items::Read(item.value());
			visuals::SpriteGrid::WriteText(LAYOUT_NAME, SPRITE_GRID_ID, { 2, row }, FONT_DEFAULT, itemDescriptor.name, color, visuals::HorizontalAlignment::LEFT);
		}
		else
		{
			visuals::SpriteGrid::WriteText(LAYOUT_NAME, SPRITE_GRID_ID, { 2, row }, FONT_DEFAULT, EMPTY, color, visuals::HorizontalAlignment::LEFT);
		}
		++row;
		++row;
	}

	static void RefreshEquipSlots()
	{
		int row = 0;
		size_t index = 0;
		for (auto& equipSlot : equipSlots)
		{
			RefreshEquipSlot(row, index, equipSlot);
			++index;
		}
	}

	static void Refresh()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID_ID);
		RefreshEquipSlots();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	static const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::BACK, ::application::UIState::Pop },
		{ ::Command::RED, ::application::UIState::Pop }
	};

	void Equipment::Start()
	{
		::application::OnEnter::AddHandler(CURRENT_STATE, OnEnter);
		::application::MouseButtonUp::AddHandler(CURRENT_STATE, OnMouseButtonUp);
		::application::Command::SetHandlers(CURRENT_STATE, commandHandlers);
		::application::Renderer::SetRenderLayout(CURRENT_STATE, LAYOUT_NAME);
	}
}
