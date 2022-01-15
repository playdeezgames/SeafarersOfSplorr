#include <Data.Game.Character.Delivery.h>
#include "Game.Session.Character.Deliveries.h"
namespace game::session::character
{
	bool Deliveries::HasAny() const
	{
		return data::game::character::Delivery::ReadCount(characterId) > 0;
	}
}