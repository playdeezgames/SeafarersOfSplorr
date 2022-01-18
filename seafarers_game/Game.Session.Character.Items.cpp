#include <algorithm>
#include <Data.Game.Character.Item.h>
#include "Game.Session.Character.Items.h"
#include "Game.Session.Items.h"
#include <iterator>
#include <numeric>
namespace game::session::character
{
	std::vector<Item> Items::GetAll() const
	{
		auto itemIds = data::game::character::Item::ReadForCharacter(characterId);
		std::vector<Item> result;
		std::transform(
			itemIds.begin(),
			itemIds.end(),
			std::back_inserter(result),
			[](int itemId) 
			{
				return Item(itemId);
			});
		return result;
	}

	std::vector<Item> Items::GetItems(const item::Type& itemType) const
	{
		auto items = GetAll();//this would be better to call its own query...
		std::vector<Item> result;
		std::copy_if(
			items.begin(),
			items.end(),
			std::back_inserter(result),
			[itemType](const Item& item) 
			{
				return item.GetItemType() == itemType;
			});
		return result;
	}

	int Items::GetItemQuantity(const item::Type& itemSubtype) const
	{
		auto items = GetItems(itemSubtype);
		return std::accumulate(
			items.begin(), 
			items.end(), 
			0, 
			[](int accumulator, Item item) 
			{
				return accumulator + item.GetQuantity();
			});
	}

	void Items::AddItemQuantity(const item::Type& itemType, int quantity) const
	{
		auto items = GetItems(itemType);
		if (!items.empty())
		{
			auto item = items.front();
			item.SetQuantity(quantity + item.GetQuantity());
			return;
		}
		auto item = game::session::Items().Add(itemType, quantity);
		item.SetCharacterId(characterId);
	}

	void Items::RemoveItemQuantity(const item::Type& itemType, int quantity) const
	{
		//TODO: gross code! 
		auto items = GetItems(itemType);
		for (auto item : items)
		{
			if (quantity > item.GetQuantity())
			{
				quantity -= item.GetQuantity();
				item.SetQuantity(0);//this is a proxy object... i can safely remove the item from the store
			}
			else
			{
				item.SetQuantity(item.GetQuantity() - quantity);
				quantity = 0;
				break;
			}
		}
	}

	bool Items::HasAny() const
	{
		return data::game::character::Item::ReadCountForCharacter(characterId) > 0;
	}
}