#include <Data.Game.Character.ItemType.h>
#include "Game.Session.Character.ItemType.h"
namespace game::session::character
{
	using ItemTypeData = data::game::character::ItemType;

	int ItemType::GetQuantity() const
	{
		return ItemTypeData::Read(characterId, itemTypeId).value_or(0);
	}

	void ItemType::RemoveQuantity(int quantity) const
	{
		auto current = GetQuantity();
		if (quantity >= current)
		{
			ItemTypeData::Clear(characterId, itemTypeId);
		}
		else
		{
			ItemTypeData::Write(characterId, itemTypeId, current - quantity);
		}
	}

}