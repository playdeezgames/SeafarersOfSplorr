#include <algorithm>
#include <Data.Game.Item.Type.Commodity.h>
#include "Game.Session.Item.Type.Commodities.h"
#include <iterator>
namespace game::session::item::type
{
	std::list<Commodity> Commodities::GetAll() const
	{
		auto commodities = data::game::item::type::Commodity::AllCommodities(itemTypeId);
		std::list<Commodity> result;
		std::transform(
			commodities.begin(),
			commodities.end(),
			std::back_inserter(result),
			[this](int commodity){
				return Commodity(itemTypeId, (game::Commodity)commodity);
			});
		return result;
	}
}

