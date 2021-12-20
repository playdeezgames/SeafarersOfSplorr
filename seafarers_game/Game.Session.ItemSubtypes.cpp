#include <Data.Game.Item.Type.h>
#include <Data.Game.Item.Type.Commodity.h>
#include "Game.Session.ItemSubtypes.h"
namespace game::session
{
	ItemSubtype ItemSubtypes::GetItemSubtype(int itemSubtypeId) const
	{
		return ItemSubtype(itemSubtypeId);
	}

	void ItemSubtypes::Reset(const game::Difficulty&) const
	{
		data::game::item::type::Commodity::Clear();
		data::game::item::Type::Clear();
	}

	ItemSubtype ItemSubtypes::Create(const game::item::Category& category, const std::string& name) const
	{
		auto index = data::game::item::Type::ReadNextSubtype((int)category);
		return ItemSubtype(
			data::game::item::Type::Establish((int)category, index, name));
	}
}