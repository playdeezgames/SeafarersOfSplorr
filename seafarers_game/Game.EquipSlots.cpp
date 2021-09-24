#include "Game.EquipSlots.h"
#include <map>
namespace game
{
	static const std::map<EquipSlot, EquipSlotDescriptor> equipSlotDescriptors = 
	{
		{EquipSlot::LEGS,
			{"legs"}},
		{EquipSlot::PRISON_WALLET,
			{"'prison wallet'"}}
	};

	static std::list<EquipSlot> equipSlots;

	const std::list<EquipSlot>& EquipSlots::All()
	{
		if (equipSlots.empty())
		{
			for (auto& entry : equipSlotDescriptors)
			{
				equipSlots.push_back(entry.first);
			}
		}
		return equipSlots;
	}

	const EquipSlotDescriptor& EquipSlots::Read(const EquipSlot& equipSlot)
	{
		return equipSlotDescriptors.find(equipSlot)->second;
	}
}