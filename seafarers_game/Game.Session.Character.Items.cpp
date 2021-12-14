#include <Data.Game.Character.ItemInstance.h>
#include "Game.Session.Character.Items.h"
namespace game::session::character
{
	Items::Items(int characterId)
		: characterId(characterId)
	{

	}

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

	void Items::Purge() const
	{
		data::game::character::ItemInstance::Purge();
	}

	void Items::Add(const Item& item) const
	{
		item.SetCharacterId(characterId);
	}
}