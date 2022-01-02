#include <Data.Game.Item.Type.h>
#include <Data.Game.Item.Type.Commodity.h>
#include "Game.Session.Item.Types.h"
namespace game::session::item
{
	using TypeData = data::game::item::Type;
	using CommodityData = data::game::item::type::Commodity;

	void Types::Reset() const
	{
		CommodityData::Clear();
		TypeData::Clear();
	}

	Type Types::Create(const game::item::Category& category, const std::string& name) const
	{
		auto index = TypeData::ReadNextTypeForCategory((int)category);
		return Type(TypeData::EstablishTypeForCategory((int)category, index, name));
	}
}