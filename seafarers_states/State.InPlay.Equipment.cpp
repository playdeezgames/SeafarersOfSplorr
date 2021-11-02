#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Utility.h>
#include <Common.Utility.Dispatcher.h>
#include <Game.Audio.Mux.h>
#include <Game.Avatar.h>
#include <Game.Avatar.Equipment.h>
#include <Game.Avatar.Items.h>
#include <Game.Colors.h>
#include <Game.EquipSlots.h>
#include <Game.Items.h>
#include "State.InPlay.CrewDetail.h"
#include "State.InPlay.Equipment.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_EQUIPMENT;
	static const std::string EMPTY = "(empty)";

	static const std::vector<game::EquipSlot> equipSlots =
	{
		game::EquipSlot::LEGS,
		game::EquipSlot::PRISON_WALLET
	};

	static void RefreshEquipSlot(size_t index, const game::EquipSlot& equipSlot)
	{
		auto slotName = game::EquipSlots::GetName(equipSlot);

		auto item = game::avatar::Equipment::Read(equipSlot);
		std::string itemName = item.has_value() ? (game::Items::GetName(item.value())) : EMPTY;
		Terminal::WriteLine("{}) {} - {}", index, slotName, itemName);
	}

	static void RefreshEquipSlots()
	{
		size_t index = 1;
		for (auto& equipSlot : equipSlots)
		{
			RefreshEquipSlot(index, equipSlot);
			++index;
		}
	}

	//static std::string GetEquippableItemName(const std::optional<game::Item>& item)
	//{
	//	return 
	//		(item.has_value()) ? 
	//		(game::Items::GetName(item.value())) : 
	//		(EMPTY);
	//}

	//static void RefreshItem(int& row, bool hilite, const std::optional<game::Item>& item)
	//{
	//	auto color = (hilite) ? (game::Colors::WHITE) : (game::Colors::DARK_GRAY);
	//	visuals::SpriteGrid::WriteText(
	//		LAYOUT_NAME, 
	//		SPRITE_GRID_ID, 
	//		{ EQUIPPABLE_ITEM_X, row }, 
	//		FONT_DEFAULT, 
	//		GetEquippableItemName(item), 
	//		color, 
	//		visuals::HorizontalAlignment::RIGHT);
	//	++row;
	//}

	//static void RefreshItems()
	//{
	//	int row = 0;
	//	size_t index=0;
	//	for (auto& item : items)
	//	{
	//		RefreshItem(row, index == itemIndex, item);
	//		index++;
	//	}
	//}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		Terminal::WriteLine("Equipment for {}:", game::Avatar::GetName(CrewDetail::GetAvatarId()).value());

		Terminal::SetForeground(game::Colors::YELLOW);
		RefreshEquipSlots();
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();

	}

	//static std::set<game::Item> DetermineCandidates(const game::EquipSlot& equipSlot)
	//{
	//	std::set<game::Item> candidates;
	//	auto current = game::avatar::Equipment::Read(equipSlot);//add current item
	//	if (current)
	//	{
	//		candidates.insert(current.value());
	//	}
	//	auto equippableItems = game::EquipSlots::GetItems(equipSlot);//add other possible items
	//	for (auto equippableItem : equippableItems)
	//	{
	//		if (game::avatar::Items::Has(equippableItem))
	//		{
	//			candidates.insert(equippableItem);
	//		}
	//	}
	//	return candidates;
	//}

	//static void DoUpdateItems(const std::set<game::Item>& candidates)
	//{
	//	items.clear();
	//	items.push_back(std::nullopt);//you can always equip nothing... thats called UNequipping!
	//	for (auto candidate : candidates)
	//	{
	//		items.push_back(candidate);
	//	}
	//}

	//static void UpdateItems()
	//{
	//	auto equipSlot = equipSlots[equipSlotIndex];
	//	DoUpdateItems(DetermineCandidates(equipSlot));
	//	UpdateItemIndex(equipSlot);
	//}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		Refresh();
	}

	//static void UnequipItem(const std::optional<game::Item>& item)
	//{
	//	if (item)
	//	{
	//		game::avatar::Items::Add(item.value(), 1);
	//		game::avatar::Equipment::Unequip(equipSlots[equipSlotIndex]);
	//	}
	//}

	//static void EquipItem(const std::optional<game::Item>& item)
	//{
	//	if (item)
	//	{
	//		game::avatar::Items::Remove(item.value(), 1);
	//		game::avatar::Equipment::Equip(equipSlots[equipSlotIndex], item.value());
	//	}
	//}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "0", application::UIState::GoTo(::UIState::IN_PLAY_CREW_DETAIL)}
	};

	static const void OnOtherInput(const std::string& line)
	{
		Terminal::ErrorMessage(Terminal::INVALID_INPUT);
		Refresh();
	}

	void Equipment::Start()
	{
		::application::OnEnter::AddHandler(
			CURRENT_STATE, 
			OnEnter);
		::application::Renderer::SetRenderLayout(
			CURRENT_STATE, 
			Terminal::LAYOUT_NAME);
		::application::Keyboard::AddHandler(
			CURRENT_STATE,
			Terminal::DoIntegerInput(
				menuActions,
				OnOtherInput));
	}
}
