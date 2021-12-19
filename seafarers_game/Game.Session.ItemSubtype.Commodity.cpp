#include <Data.Game.ItemSubtype.Commodity.h>
#include "Game.Session.ItemSubtype.Commodity.h"
namespace game::session::item_subtype
{
	void Commodity::SetAmount(double amount) const
	{
		data::game::item_subtype::Commodity::Write(itemSubtypeId, (int)commodity, amount);
	}
}