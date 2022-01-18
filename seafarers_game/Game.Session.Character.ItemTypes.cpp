#include <algorithm>
#include <Data.Game.Character.ItemType.h>
#include "Game.Session.Character.ItemTypes.h"
#include <iterator>
namespace game::session::character
{
	std::vector<ItemType> ItemTypes::GetItemTypes() const
	{
		auto itemTypes = data::game::character::ItemType::ReadForCharacter(characterId);
		std::vector<ItemType> result;
		std::transform(
			itemTypes.begin(), 
			itemTypes.end(), 
			std::back_inserter(result), 
			[this](int itemTypeId) 
			{
				return ItemType(characterId, itemTypeId);
			});
		return result;
	}

	void ItemTypes::AddItemTypeQuantity(const item::Type& itemType, int delta) const
	{
		auto quantity = data::game::character::ItemType::Read(characterId, itemType.operator int()).value_or(0);
		quantity += delta;
		if (quantity > 0)
		{
			data::game::character::ItemType::Write(characterId, itemType.operator int(), quantity);
		}
		else
		{
			data::game::character::ItemType::Clear(characterId, itemType.operator int());
		}
	}

	void ItemTypes::RemoveItemTypeQuantity(const item::Type& itemType, int delta) const
	{
		AddItemTypeQuantity(itemType, -delta);
	}

	bool ItemTypes::HasAny() const
	{
		return data::game::character::ItemType::ReadCountForCharacter(characterId).value_or(0) > 0;
	}

}