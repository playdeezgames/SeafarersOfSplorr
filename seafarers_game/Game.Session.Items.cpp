#include <Data.Game.ItemInstance.h>
#include "Game.Session.Items.h"
namespace game::session
{
	Item Items::GetItem(int itemInstanceId) const
	{
		return Item(itemInstanceId);
	}

	void Items::Purge() const
	{
		data::game::ItemInstance::Purge();
	}

	Item Items::Add(const game::ItemType& itemType, int subtype, int quantity) const
	{
		int itemInstanceId = data::game::ItemInstance::Create((int)itemType, subtype, quantity);
		return Item(itemInstanceId);
	}

}