#include "Game.Session.ItemSubtypes.h"
namespace game::session
{
	ItemType ItemTypes::GetItemType(int itemSubtypeId) const
	{
		return ItemType(itemSubtypeId);
	}
}