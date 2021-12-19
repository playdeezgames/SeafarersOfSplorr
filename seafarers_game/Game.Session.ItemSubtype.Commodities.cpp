#include <Data.Game.ItemSubtype.Commodity.h>
#include "Game.Session.ItemSubtype.Commodities.h"
namespace game::session::item_subtype
{
	std::list<Commodity> Commodities::GetAll() const
	{
		auto commodities = data::game::item_subtype::Commodity::All(itemSubtypeId);
		std::list<Commodity> result;
		for (auto commodity : commodities)
		{
			result.push_back(Commodity(itemSubtypeId, (game::Commodity)commodity));
		}
		return result;
	}
}

