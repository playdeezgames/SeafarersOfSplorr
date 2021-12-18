#include <Data.Game.ItemInstance.h>
#include <Data.Game.Character.ItemInstance.h>
#include "Game.Session.Item.h"
namespace game::session
{
	ItemSubtype Item::GetItemSubtype() const
	{
		return ItemSubtype(data::game::ItemInstance::ReadItemSubtypeId(itemInstanceId).value());
	}

	void Item::SetQuantity(int quantity) const
	{
		data::game::ItemInstance::WriteQuantity(itemInstanceId, quantity);
	}

	int Item::GetQuantity() const
	{
		return data::game::ItemInstance::ReadQuantity(itemInstanceId).value();
	}

	std::optional<int> Item::TryGetCharacterId() const
	{
		return data::game::character::ItemInstance::ReadForItemInstance(itemInstanceId);
	}

	int Item::GetCharacterId() const
	{
		return TryGetCharacterId().value();
	}

	void Item::SetCharacterId(int characterId) const
	{
		data::game::character::ItemInstance::Write(itemInstanceId, characterId);
	}
}