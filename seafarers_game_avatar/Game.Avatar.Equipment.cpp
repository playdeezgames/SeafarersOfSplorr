#include <Common.Utility.h>
#include <Common.Utility.Table.h>
#include <Data.Game.Avatar.Equipment.h>
#include "Game.Avatar.Equipment.h"
#include <Game.Player.h>
namespace game::avatar//20211017
{
	void Equipment::Reset(const Difficulty& difficulty)
	{
		data::game::avatar::Equipment::Clear(Player::GetAvatarId());
	}

	std::optional<Item> Equipment::Read(const EquipSlot& equipSlot)
	{
		auto item = data::game::avatar::Equipment::Read(Player::GetAvatarId(), (int)equipSlot);
		if (item)
		{
			return (Item)item.value();
		}
		return std::nullopt;
	}

	void Equipment::Equip(const EquipSlot& equipSlot, const Item& item)
	{
		data::game::avatar::Equipment::Write(Player::GetAvatarId(), (int)equipSlot, (int)item);
	}

	void Equipment::Unequip(const EquipSlot& equipSlot)
	{
		data::game::avatar::Equipment::Write(Player::GetAvatarId(), (int)equipSlot, std::nullopt);
	}

	std::map<EquipSlot, Item> Equipment::All()
	{
		return 
			common::utility::Table::MapTable<int, int, EquipSlot, Item>(
				data::game::avatar::Equipment::All(Player::GetAvatarId()), 
				common::Utility::Cast<int, EquipSlot>,
				common::Utility::Cast<int, Item>);
	}

	bool Equipment::IsEquipped(const Item& item)
	{
		for (auto& entry : All())
		{
			if (entry.second == item)
			{
				return true;
			}
		}
		return false;
	}

}