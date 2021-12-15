#include "Game.Session.ItemTypes.h"
namespace game::session
{
	ItemType ItemTypes::GetItemType(int itemSubtypeId) const
	{
		return ItemType(itemSubtypeId);
	}
}