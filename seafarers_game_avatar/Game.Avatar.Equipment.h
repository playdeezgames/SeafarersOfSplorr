#pragma once
#include "Game.Difficulty.h"
#include "Game.EquipSlot.h"
#include "Game.Item.h"
#include <map>
#include <optional>
namespace game::avatar
{
	struct Equipment
	{
		static void Reset(const Difficulty&);
		static std::optional<Item> Read(const EquipSlot&);
		static void Equip(const EquipSlot&, const Item&);
		static void Unequip(const EquipSlot&);
		static std::map<EquipSlot, Item> All();
	};
}