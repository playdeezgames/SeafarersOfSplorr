#include <Common.Utility.List.h>
#include "Game.EquipSlots.h"
#include <map>
namespace game
{
	struct EquipSlotDescriptor
	{
		std::string name;
	};

	static const std::map<EquipSlot, EquipSlotDescriptor> equipSlotDescriptors = 
	{
		{EquipSlot::LEGS,
			{
				"legs"
			}},
		{EquipSlot::PRISON_WALLET,
			{
				"'prison wallet'"
			}}
	};

	static std::list<EquipSlot> equipSlots;

	const std::list<EquipSlot>& EquipSlots::All()
	{
		return common::utility::List::FromTable(equipSlots, equipSlotDescriptors);
	}

	static const EquipSlotDescriptor& Read(const EquipSlot& equipSlot)
	{
		return equipSlotDescriptors.find(equipSlot)->second;
	}

	const std::string& EquipSlots::GetName(const EquipSlot& equipSlot)
	{
		return Read(equipSlot).name;
	}
}