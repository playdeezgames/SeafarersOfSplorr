#include <Data.Game.Character.ItemInstance.h>
#include "Game.Session.Character.Items.h"
#include "Game.Session.Items.h"
namespace game::session::character
{
	std::list<Item> Items::GetAll() const
	{
		auto itemInstances = data::game::character::ItemInstance::ReadForCharacter(characterId);
		std::list<Item> result;
		for (auto itemInstanceId : itemInstances)
		{
			result.push_back(Item(itemInstanceId));
		}
		return result;
	}

	std::list<Item> Items::GetItems(const item::Type& itemSubtype) const
	{
		auto items = GetAll();
		std::list<Item> result;
		for (auto item : items)
		{
			if (item.GetItemSubtype()==itemSubtype)
			{
				result.push_back(item);
			}
		}
		return result;
	}

	int Items::GetItemQuantity(const item::Type& itemSubtype) const
	{
		int quantity = 0;
		auto items = GetItems(itemSubtype);
		for (auto item : items)
		{
			quantity += item.GetQuantity();
		}
		return quantity;
	}

	void Items::AddItemQuantity(const item::Type& itemSubtype, int quantity) const
	{
		auto items = GetItems(itemSubtype);
		for (auto item : items)
		{
			item.SetQuantity(quantity + item.GetQuantity());
			break;
		}
		auto item = game::session::Items().Add(itemSubtype, quantity);
		item.SetCharacterId(characterId);
	}

	void Items::RemoveItemQuantity(const item::Type& itemSubtype, int quantity) const
	{
		auto items = GetItems(itemSubtype);
		for (auto item : items)
		{
			if (quantity > item.GetQuantity())
			{
				quantity -= item.GetQuantity();
				item.SetQuantity(0);
			}
			else
			{
				item.SetQuantity(item.GetQuantity() - quantity);
				quantity = 0;
				break;
			}
		}
	}
}