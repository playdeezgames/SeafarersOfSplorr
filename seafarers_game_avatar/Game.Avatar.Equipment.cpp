#include <Common.Utility.h>
#include <Common.Utility.Table.h>
#include <Data.Game.Character.Equipment.h>
#include "Game.Avatar.Equipment.h"
#include <Game.Player.h>
namespace game::avatar
{
	void Equipment::Reset(const Difficulty& difficulty)
	{
		data::game::character::Equipment::Clear(Player::GetAvatarId());
	}

	std::optional<Item> Equipment::Read(int avatarId, const EquipSlot& equipSlot)
	{
		auto item = data::game::character::Equipment::Read(avatarId, (int)equipSlot);
		if (item)
		{
			return (Item)item.value();
		}
		return std::nullopt;
	}

	void Equipment::Equip(int avatarId, const EquipSlot& equipSlot, const Item& item)
	{
		data::game::character::Equipment::Write(avatarId, (int)equipSlot, (int)item);
	}

	void Equipment::Unequip(int avatarId, const EquipSlot& equipSlot)
	{
		data::game::character::Equipment::Write(avatarId, (int)equipSlot, std::nullopt);
	}

	std::map<EquipSlot, Item> Equipment::All(int avatarId)
	{
		return 
			common::utility::Table::Map<int, int, EquipSlot, Item>(
				data::game::character::Equipment::All(avatarId),
				common::Utility::Cast<int, EquipSlot>,
				common::Utility::Cast<int, Item>);
	}

	bool Equipment::IsEquipped(int avatarId, const Item& item)
	{
		for (auto& entry : All(avatarId))
		{
			if (entry.second == item)
			{
				return true;
			}
		}
		return false;
	}

}