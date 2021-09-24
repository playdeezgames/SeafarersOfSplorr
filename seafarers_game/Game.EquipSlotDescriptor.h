#pragma once
#include "Game.Item.h"
#include <set>
#include <string>
namespace game
{
	struct EquipSlotDescriptor
	{
		std::string name;
		std::set<Item> items;
	};
}
