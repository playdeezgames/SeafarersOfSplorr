#include <Data.Game.Item.Type.h>
#include "Game.Session.ItemSubtype.h"
namespace game::session
{
	std::string ItemSubtype::GetName() const
	{
		return data::game::item::Type::ReadName(itemSubtypeId).value();
	}

}