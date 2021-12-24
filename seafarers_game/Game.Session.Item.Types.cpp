#include <Data.Game.Item.Type.h>
#include <Data.Game.Item.Type.Commodity.h>
#include "Game.Session.Item.Types.h"
namespace game::session::item
{
	Type Types::GetItemSubtype(int itemSubtypeId) const
	{
		return Type(itemSubtypeId);
	}

	void Types::Reset(const game::Difficulty&) const
	{
		data::game::item::type::Commodity::Clear();
		data::game::item::Type::Clear();
	}

	Type Types::Create(const game::item::Category& category, const std::string& name) const
	{
		auto index = data::game::item::Type::ReadNextTypeForCategory((int)category);
		return Type(data::game::item::Type::Establish((int)category, index, name));
	}
}