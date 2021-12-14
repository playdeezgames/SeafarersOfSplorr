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

	Item Items::Add(const game::ItemType& itemType, int count) const
	{
		int itemInstanceId = data::game::ItemInstance::Create((int)itemType, count);
		return Item(itemInstanceId);
	}

}