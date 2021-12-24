#include <Data.Game.Item.h>
#include <Data.Game.Character.ItemInstance.h>
#include "Game.Session.Item.h"
namespace game::session
{
	item::Type Item::GetItemSubtype() const
	{
		return item::Type(data::game::ItemInstance::ReadItemTypeId(itemId).value());
	}

	void Item::SetQuantity(int quantity) const
	{
		data::game::ItemInstance::WriteQuantity(itemId, quantity);
	}

	int Item::GetQuantity() const
	{
		return data::game::ItemInstance::ReadQuantity(itemId).value();
	}

	std::optional<int> Item::TryGetCharacterId() const
	{
		return data::game::character::ItemInstance::ReadForItemInstance(itemId);
	}

	int Item::GetCharacterId() const
	{
		return TryGetCharacterId().value();
	}

	void Item::SetCharacterId(int characterId) const
	{
		data::game::character::ItemInstance::Write(itemId, characterId);
	}
}