#include <Application.Keyboard.h>
#include <Application.OnEnter.h>
#include <Application.Renderer.h>
#include <Application.UIState.h>
#include <Common.Data.h>
#include <Game.Audio.Mux.h>
#include <Game.Character.h>
#include <Game.Character.Equipment.h>
#include <Game.Character.Items.h>
#include <Game.Colors.h>
#include <Game.EquipSlots.h>
#include <Game.Items.h>
#include "State.InPlay.CrewDetail.h"
#include "State.InPlay.EquipmentSlot.h"
#include "State.Terminal.h"
#include "UIState.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_EQUIPMENT_SLOT;
	static game::EquipSlot equipmentSlot;
	static std::vector<std::optional<game::Item>> items;
	static std::string NOTHING = "(nothing)";

	static void RefreshItems()
	{
		int index = 1;
		for (auto item : items)
		{
			std::string itemName = (item.has_value()) ? (game::Items::GetName(item.value())) : NOTHING;
			Terminal::WriteLine("{}) {}", index++, itemName);
		}
	}

	static void Refresh()
	{
		Terminal::Reinitialize();

		Terminal::SetForeground(game::Colors::LIGHT_CYAN);
		auto avatarName = game::Character::GetName(CrewDetail::GetCharacterId()).value();
		auto equipSlotName = game::EquipSlots::GetName(EquipmentSlot::GetSlot());
		Terminal::WriteLine("Equipping {}'s {}:", avatarName, equipSlotName);
		Terminal::SetForeground(game::Colors::GRAY);
		auto item = game::character::Equipment::Read(CrewDetail::GetCharacterId(), equipmentSlot);
		std::string itemName = item.has_value() ? (game::Items::GetName(item.value())) : NOTHING;
		Terminal::WriteLine("Currently Equipped: {}", itemName);
		Terminal::SetForeground(game::Colors::YELLOW);
		RefreshItems();
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();
	}

	void EquipmentSlot::SetSlot(const game::EquipSlot& slot)
	{
		equipmentSlot = slot;
	}

	game::EquipSlot EquipmentSlot::GetSlot()
	{
		return equipmentSlot;
	}

	static std::set<game::Item> DetermineCandidates()
	{
		std::set<game::Item> candidates;
		auto current = game::character::Equipment::Read(CrewDetail::GetCharacterId(), equipmentSlot);//add current item
		if (current)
		{
			candidates.insert(current.value());
		}
		auto equippableItems = game::EquipSlots::GetItems(equipmentSlot);//add other possible items
		for (auto equippableItem : equippableItems)
		{
			if (game::character::Items::Has(CrewDetail::GetCharacterId(), equippableItem))
			{
				candidates.insert(equippableItem);
			}
		}
		return candidates;
	}


	static void UpdateItems()
	{
		std::set<game::Item> candidates = DetermineCandidates();
		items.clear();
		items.push_back(std::nullopt);//you can always equip nothing... thats called UNequipping!
		for (auto candidate : candidates)
		{
			items.push_back(candidate);
		}
	}

	static void UnequipItem(const std::optional<game::Item>& item)
	{
		if (item)
		{
			//TODO: write to terminal that item was unequipped
			game::character::Items::Add(CrewDetail::GetCharacterId(), item.value(), 1);
			game::character::Equipment::Unequip(CrewDetail::GetCharacterId(), equipmentSlot);
		}
	}

	static void EquipItem(const std::optional<game::Item>& item)
	{
		if (item)
		{
			//TODO: write to terminal that item was equipped
			game::character::Items::Remove(CrewDetail::GetCharacterId(), item.value(), 1);
			game::character::Equipment::Equip(CrewDetail::GetCharacterId(), equipmentSlot, item.value());
		}
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Theme::MAIN);
		UpdateItems();
		Refresh();
	}

	static const std::map<std::string, std::function<void()>> menuActions =
	{
		{ "0", application::UIState::GoTo(::UIState::IN_PLAY_EQUIPMENT)}
	};

	static const void OnOtherInput(const std::string& line)
	{
		int index = common::Data::ToInt(line) - 1;
		if (index >= 0 && index < items.size())
		{
			auto newItem = items[index];
			auto oldItem = game::character::Equipment::Read(CrewDetail::GetCharacterId(), equipmentSlot);
			UnequipItem(oldItem);
			EquipItem(newItem);
			application::UIState::Write(::UIState::IN_PLAY_EQUIPMENT);
		}
		else
		{
			Terminal::ErrorMessage(Terminal::INVALID_INPUT);
			Refresh();
		}
	}

	void EquipmentSlot::Start()
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