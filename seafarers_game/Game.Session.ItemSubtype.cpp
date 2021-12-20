#include <Data.Game.ItemSubtype.h>
#include "Game.Session.ItemSubtype.h"
namespace game::session
{
	std::string ItemSubtype::GetName() const
	{
		return data::game::ItemSubtype::ReadName(itemSubtypeId).value();
	}

}