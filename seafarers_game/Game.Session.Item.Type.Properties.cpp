#include "Game.Session.Item.Type.Properties.h"
#include <Data.Game.Item.Type.Property.h>
namespace game::session::item::type
{
	void Properties::AddProperty(const std::string& name, int value) const
	{
		data::game::item::type::Property::WriteInt(itemTypeId, name, value);
	}
}