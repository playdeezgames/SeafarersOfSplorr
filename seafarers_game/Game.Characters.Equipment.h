#pragma once
#include "Game.Difficulty.h"
#include "Game.EquipSlot.h"
#include "Game.ItemLegacy.h"
#include <map>
#include <optional>
namespace game::characters
{
	struct Equipment
	{
		static void Reset(const Difficulty&);
		static std::optional<Item> Read(int, const EquipSlot&);
		static void Equip(int, const EquipSlot&, const Item&);
		static void Unequip(int, const EquipSlot&);
		static std::map<EquipSlot, Item> All(int);
		static bool IsEquipped(int, const Item&);
	};
}