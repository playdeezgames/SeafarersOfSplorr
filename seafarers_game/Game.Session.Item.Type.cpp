#include <Data.Game.Item.Type.h>
#include "Game.Session.Item.Type.h"
namespace game::session::item
{
	std::string Type::GetName() const
	{
		return data::game::item::Type::ReadName(typeId).value();
	}

}