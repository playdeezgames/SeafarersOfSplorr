#include <Data.Game.Avatar.Equipment.h>
#include "Game.Avatar.Equipment.h"
namespace game::avatar
{
	void Equipment::Reset(const Difficulty& difficulty)
	{
		data::game::avatar::Equipment::Clear();
	}

	std::optional<Item> Equipment::Read(const EquipSlot& equipSlot)
	{
		return std::nullopt;
	}

	void Equipment::Equip(const EquipSlot& equipSlot, const Item& item)
	{
		data::game::avatar::Equipment::Write((int)equipSlot, (int)item);
	}

	void Equipment::Unequip(const EquipSlot& equipSlot)
	{
		data::game::avatar::Equipment::Write((int)equipSlot, std::nullopt);
	}

	std::map<EquipSlot, Item> Equipment::All()
	{
		std::map<EquipSlot, Item> result;
		return result;
	}
}