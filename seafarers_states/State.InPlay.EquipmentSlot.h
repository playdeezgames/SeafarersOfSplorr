#pragma once
#include <Game.EquipSlot.h>
namespace state::in_play
{
	struct EquipmentSlot
	{
		static void SetSlot(const game::EquipSlot&);
		static game::EquipSlot GetSlot();
		static void Start();
	};
}