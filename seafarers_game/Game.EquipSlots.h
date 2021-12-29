#pragma once
#include "Game.EquipSlot.h"
#include <list>
#include <set>
#include <string>
namespace game
{
	struct EquipSlots
	{
		static const std::list<EquipSlot>& All();
		static const std::string& GetName(const EquipSlot&);
	};
}