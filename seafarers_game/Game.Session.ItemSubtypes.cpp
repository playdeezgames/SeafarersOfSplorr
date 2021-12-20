#include <Data.Game.ItemSubtype.h>
#include <Data.Game.ItemSubtype.Commodity.h>
#include "Game.Session.ItemSubtypes.h"
namespace game::session
{
	ItemSubtype ItemSubtypes::GetItemSubtype(int itemSubtypeId) const
	{
		return ItemSubtype(itemSubtypeId);
	}

	void ItemSubtypes::Reset(const game::Difficulty&) const
	{
		data::game::item_subtype::Commodity::Clear();
		data::game::ItemSubtype::Clear();
	}

	ItemSubtype ItemSubtypes::Create(const game::item::Category& category, const std::string& name) const
	{
		auto index = data::game::ItemSubtype::ReadNextSubtype((int)category);
		return ItemSubtype(
			data::game::ItemSubtype::Establish((int)category, index, name));
	}
}