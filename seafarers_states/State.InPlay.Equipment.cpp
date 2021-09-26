#include <Application.Renderer.h>
#include <Application.Command.h>
#include <Application.MouseButtonUp.h>
#include <Application.OnEnter.h>
#include <Application.UIState.h>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.Equipment.h>
#include <Game.Avatar.Items.h>
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
	static const int EQUIP_SLOT_X = 0;
	static const int EQUIPPED_ITEM_X = 2;
	static const int EQUIPPABLE_ITEM_X = 39;

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		::application::UIState::Pop();
		return true;
	}

	static const std::vector<game::EquipSlot> equipSlots =
	{
		game::EquipSlot::LEGS,
		game::EquipSlot::PRISON_WALLET
	};
	static size_t equipSlotIndex = 0;

	static void RefreshEquipSlot(int& row, size_t index, const game::EquipSlot& equipSlot)
	{
		auto color = (index == equipSlotIndex) ? (game::Colors::WHITE) : (game::Colors::DARK_GRAY);//TODO: just pass in a bool
		auto equipSlotDescriptor = game::EquipSlots::Read(equipSlot);
		visuals::SpriteGrid::WriteText(LAYOUT_NAME, SPRITE_GRID_ID, { EQUIP_SLOT_X, row }, FONT_DEFAULT, equipSlotDescriptor.name, color, visuals::HorizontalAlignment::LEFT);
		++row;

		auto item = game::avatar::Equipment::Read(equipSlot);
		if (item)
		{
			auto itemDescriptor = game::Items::Read(item.value());
			visuals::SpriteGrid::WriteText(LAYOUT_NAME, SPRITE_GRID_ID, { EQUIPPED_ITEM_X, row }, FONT_DEFAULT, itemDescriptor.name, color, visuals::HorizontalAlignment::LEFT);
		}
		else
		{
			visuals::SpriteGrid::WriteText(LAYOUT_NAME, SPRITE_GRID_ID, { EQUIPPED_ITEM_X, row }, FONT_DEFAULT, EMPTY, color, visuals::HorizontalAlignment::LEFT);
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

	static std::vector<std::optional<game::Item>> items;
	static size_t itemIndex;

	static std::string GetEquippableItemName(const std::optional<game::Item>& item)
	{
		return 
			(item.has_value()) ? 
			(game::Items::Read(item.value()).name) : 
			(EMPTY);
	}

	static void RefreshItem(int& row, bool hilite, const std::optional<game::Item>& item)
	{
		auto color = (hilite) ? (game::Colors::WHITE) : (game::Colors::DARK_GRAY);
		visuals::SpriteGrid::WriteText(
			LAYOUT_NAME, 
			SPRITE_GRID_ID, 
			{ EQUIPPABLE_ITEM_X, row }, 
			FONT_DEFAULT, 
			GetEquippableItemName(item), 
			color, 
			visuals::HorizontalAlignment::RIGHT);
		++row;
	}

	static void RefreshItems()
	{
		int row = 0;
		size_t index=0;
		for (auto& item : items)
		{
			RefreshItem(row, index == itemIndex, item);
			index++;
		}
	}

	static void Refresh()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID_ID);
		RefreshEquipSlots();
		RefreshItems();
	}

	static std::set<game::Item> DetermineCandidates(const game::EquipSlot& equipSlot)
	{
		std::set<game::Item> candidates;
		auto current = game::avatar::Equipment::Read(equipSlot);//add current item
		if (current)
		{
			candidates.insert(current.value());
		}
		auto equipSlotDescriptor = game::EquipSlots::Read(equipSlot);//add other possible items
		for (auto equippableItem : equipSlotDescriptor.items)
		{
			if (game::avatar::Items::Has(equippableItem))
			{
				candidates.insert(equippableItem);
			}
		}
		return candidates;
	}

	static void UpdateItemIndex(const game::EquipSlot& equipSlot)
	{
		auto current = game::avatar::Equipment::Read(equipSlot);
		itemIndex = 0;
		while (items[itemIndex] != current)
		{
			itemIndex++;
		}
	}

	static void DoUpdateItems(const std::set<game::Item>& candidates)
	{
		items.clear();
		items.push_back(std::nullopt);//you can always equip nothing... thats called UNequipping!
		for (auto candidate : candidates)
		{
			items.push_back(candidate);
		}
	}

	static void UpdateItems()
	{
		auto equipSlot = equipSlots[equipSlotIndex];
		DoUpdateItems(DetermineCandidates(equipSlot));
		UpdateItemIndex(equipSlot);
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		UpdateItems();
		Refresh();
	}

	static void PreviousSlot()
	{
		equipSlotIndex = (equipSlotIndex + equipSlots.size() - 1) % equipSlots.size();
		UpdateItems();
		Refresh();
	}


	static void NextSlot()
	{
		equipSlotIndex = (equipSlotIndex + 1) % equipSlots.size();
		UpdateItems();
		Refresh();
	}

	static void UnequipItem(const std::optional<game::Item>& item)
	{
		if (item)
		{
			game::avatar::Items::Add(item.value(), 1);
			game::avatar::Equipment::Unequip(equipSlots[equipSlotIndex]);
		}
	}

	static void EquipItem(const std::optional<game::Item>& item)
	{
		if (item)
		{
			game::avatar::Items::Remove(item.value(), 1);
			game::avatar::Equipment::Equip(equipSlots[equipSlotIndex], item.value());
		}
	}

	static void SetItemIndex(size_t newItemIndex)
	{
		auto oldItem = items[itemIndex];
		itemIndex = newItemIndex;
		auto newItem = items[itemIndex];
		UnequipItem(oldItem);
		EquipItem(newItem);
		UpdateItems();
		Refresh();
	}

	static void NextItem()
	{
		SetItemIndex((itemIndex + 1) % items.size());
	}

	static const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::YELLOW, NextItem},
		{ ::Command::UP, PreviousSlot},
		{ ::Command::DOWN, NextSlot},
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
