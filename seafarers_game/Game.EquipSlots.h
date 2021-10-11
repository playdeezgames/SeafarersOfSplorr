#pragma once
#include "Game.EquipSlot.h"
#include "Game.Item.h"
#include <list>
#include <set>
#include <string>
namespace game//20211011
{
	struct EquipSlots
	{
		static const std::list<EquipSlot>& All();
		static const std::string& GetName(const EquipSlot&);
		static const std::set<Item>& GetItems(const EquipSlot&);
	};
}