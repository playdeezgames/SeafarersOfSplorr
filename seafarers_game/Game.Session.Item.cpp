#include <Data.Game.Item.h>
#include <Data.Game.Character.Item.h>
#include "Game.Session.Item.h"
namespace game::session
{
	item::Type Item::GetItemSubtype() const
	{
		return item::Type(data::game::Item::ReadItemTypeId(itemId).value());
	}

	void Item::SetQuantity(int quantity) const
	{
		data::game::Item::WriteQuantity(itemId, quantity);
	}

	int Item::GetQuantity() const
	{
		return data::game::Item::ReadQuantity(itemId).value();
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