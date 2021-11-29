#include <Common.Utility.h>
#include <Common.Utility.Table.h>
#include <Data.Game.Character.Equipment.h>
#include "Game.Character.Equipment.h"
namespace game::character
{
	void Equipment::Reset(const Difficulty& difficulty)
	{
		data::game::character::Equipment::Clear();
	}

	std::optional<Item> Equipment::Read(int characterId, const EquipSlot& equipSlot)
	{
		auto item = data::game::character::Equipment::Read(characterId, (int)equipSlot);
		if (item)
		{
			return (Item)item.value();
		}
		return std::nullopt;
	}

	void Equipment::Equip(int characterId, const EquipSlot& equipSlot, const Item& item)
	{
		data::game::character::Equipment::Write(characterId, (int)equipSlot, (int)item);
	}

	void Equipment::Unequip(int characterId, const EquipSlot& equipSlot)
	{
		data::game::character::Equipment::Write(characterId, (int)equipSlot, std::nullopt);
	}

	std::map<EquipSlot, Item> Equipment::All(int characterId)
	{
		return 
			common::utility::Table::Map<int, int, EquipSlot, Item>(
				data::game::character::Equipment::All(characterId),
				common::Utility::Cast<int, EquipSlot>,
				common::Utility::Cast<int, Item>);
	}

	bool Equipment::IsEquipped(int characterId, const Item& item)
	{
		for (auto& entry : All(characterId))
		{
			if (entry.second == item)
			{
				return true;
			}
		}
		return false;
	}

}