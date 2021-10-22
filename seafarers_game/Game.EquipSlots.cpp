#include <Common.Utility.h>
#include <Common.Utility.List.h>
#include "Game.EquipSlots.h"
#include <map>
namespace game//20211015
{
	struct EquipSlotDescriptor
	{
		std::string name;
		std::set<Item> items;
	};

	static const std::map<EquipSlot, EquipSlotDescriptor> equipSlotDescriptors = 
	{
		{EquipSlot::LEGS,
			{
				"legs",
				{
					Item::TROUSERS,
					Item::POSH_TROUSERS
				}
			}},
		{EquipSlot::PRISON_WALLET,
			{
				"'prison wallet'",
				{
					Item::BAIT,
					Item::JOOLS,
					Item::SEAWEED
				}
			}}
	};

	static std::list<EquipSlot> equipSlots;

	const std::list<EquipSlot>& EquipSlots::All()
	{
		return common::utility::List::ListFromTable(equipSlots, equipSlotDescriptors);
	}

	static const EquipSlotDescriptor& Read(const EquipSlot& equipSlot)
	{
		return equipSlotDescriptors.find(equipSlot)->second;
	}

	const std::string& EquipSlots::GetName(const EquipSlot& equipSlot)
	{
		return Read(equipSlot).name;
	}

	const std::set<Item>& EquipSlots::GetItems(const EquipSlot& equipSlot)
	{
		return Read(equipSlot).items;
	}
}