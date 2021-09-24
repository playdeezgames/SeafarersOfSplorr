#pragma once
#include "Game.EquipSlot.h"
#include "Game.EquipSlotDescriptor.h"
#include <list>
namespace game
{
	struct EquipSlots
	{
		static const std::list<EquipSlot>& All();
		static const EquipSlotDescriptor& Read(const EquipSlot&);
	};
}