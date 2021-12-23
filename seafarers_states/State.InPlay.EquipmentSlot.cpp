#include <Common.Data.h>
#include <Game.Characters.Items.h>
#include <Game.EquipSlots.h>
#include <Game.Items.h>
#include <Game.Session.h>
#include "State.InPlay.CrewDetail.h"
#include "State.InPlay.Globals.h"
#include "State.InPlay.EquipmentSlot.h"
namespace state::in_play
{
	static const ::UIState CURRENT_STATE = ::UIState::IN_PLAY_EQUIPMENT_SLOT;
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
		auto avatarName = game::Session().GetCharacters().GetCharacter(GetCrewDetailCharacterId()).GetName();
		auto equipSlotName = game::EquipSlots::GetName(GetEquipmentSlot());
		Terminal::WriteLine("Equipping {}'s {}:", avatarName, equipSlotName);
		Terminal::SetForeground(game::Colors::GRAY);
		std::string itemName = NOTHING;
		Terminal::WriteLine("Currently Equipped: {}", itemName);
		Terminal::SetForeground(game::Colors::YELLOW);
		RefreshItems();
		Terminal::WriteLine("0) Never mind");

		Terminal::ShowPrompt();
	}

	static std::set<game::Item> DetermineCandidates()
	{
		std::set<game::Item> candidates;
		auto equippableItems = game::EquipSlots::GetItems(GetEquipmentSlot());//add other possible items
		for (auto equippableItem : equippableItems)
		{
			if (game::characters::Items::Has(GetCrewDetailCharacterId(), equippableItem))
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
			game::characters::Items::Add(GetCrewDetailCharacterId(), item.value(), 1);
		}
	}

	static void EquipItem(const std::optional<game::Item>& item)
	{
		if (item)
		{
			//TODO: write to terminal that item was equipped
			game::characters::Items::Remove(GetCrewDetailCharacterId(), item.value(), 1);
		}
	}

	static void OnEnter()
	{
		PlayMainTheme();
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
			auto oldItem = items[index];
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