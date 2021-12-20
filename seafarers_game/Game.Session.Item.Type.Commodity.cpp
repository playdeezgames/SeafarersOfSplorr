#include <Data.Game.Item.Type.Commodity.h>
#include "Game.Session.Item.Type.Commodity.h"
namespace game::session::item::type
{
	void Commodity::SetAmount(double amount) const
	{
		data::game::item::type::Commodity::Write(itemSubtypeId, (int)commodity, amount);
	}

	double Commodity::GetAmount() const
	{
		return data::game::item::type::Commodity::Read(itemSubtypeId, (int)commodity).value();
	}
}