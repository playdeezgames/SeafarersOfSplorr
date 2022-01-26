#include "Game.Session.Item.Types.h"
#include <Data.Game.Item.Type.Category.h>
#include <Data.Game.Item.Type.Commodity.h>
#include <Data.Game.Item.Type.h>
namespace game::session::item
{
	using TypeData = data::game::item::Type;
	using CommodityData = data::game::item::type::Commodity;
	using CategoryData = data::game::item::type::Category;

	void Types::Reset() const
	{
		CommodityData::Clear();
		TypeData::Clear();
	}

	Type Types::Create(const game::item::Category& category, const std::string& name) const
	{
		auto index = TypeData::Create(name);
		CategoryData::AddToCategory(index, (int)category);
		return Type(index);
	}
}