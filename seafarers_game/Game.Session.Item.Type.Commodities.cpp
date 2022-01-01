#include <Data.Game.Item.Type.Commodity.h>
#include "Game.Session.Item.Type.Commodities.h"
namespace game::session::item::type
{
	std::list<Commodity> Commodities::GetAll() const
	{
		auto commodities = data::game::item::type::Commodity::AllCommodities(itemSubtypeId);
		std::list<Commodity> result;
		for (auto commodity : commodities)
		{
			result.push_back(Commodity(itemSubtypeId, (game::Commodity)commodity));
		}
		return result;
	}
}

