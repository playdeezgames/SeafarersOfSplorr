#include "Game.Session.ItemSubtypes.h"
namespace game::session
{
	ItemSubtype ItemTypes::GetItemSubtype(int itemSubtypeId) const
	{
		return ItemSubtype(itemSubtypeId);
	}
}