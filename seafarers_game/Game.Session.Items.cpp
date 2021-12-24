#include <Data.Game.Item.h>
#include "Game.Session.Items.h"
namespace game::session
{
	using ItemData = data::game::Item;

	Item Items::GetItem(int itemInstanceId) const
	{
		return Item(itemInstanceId);
	}

	void Items::Purge() const
	{
		ItemData::Purge();
	}

	Item Items::Add(const item::Type& itemSubtype, int quantity) const
	{
		int itemInstanceId = ItemData::Create((int)itemSubtype, quantity);
		return Item(itemInstanceId);
	}

}