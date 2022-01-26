#include "Game.Session.Item.Type.h"
#include <Data.Game.Item.Type.Category.h>
#include <Data.Game.Item.Type.h>
namespace game::session::item
{
	std::string Type::GetName() const
	{
		return data::game::item::Type::ReadName(typeId).value();
	}

	bool Type::HasCategory(const game::item::Category& category) const
	{
		return data::game::item::type::Category::HasCategory(typeId, (int)category);
	}

}