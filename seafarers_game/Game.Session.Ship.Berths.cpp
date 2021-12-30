#include <algorithm>
#include <Data.Game.Character.Ship.h>
#include "Game.Session.Ship.Berths.h"
#include <iterator>
namespace game::session::ship
{
	std::list<Berth> Berths::GetBerths() const
	{
		auto characterIds = data::game::character::Ship::ReadCharactersForShip(shipId);
		std::list<Berth> result;
		std::transform(
			characterIds.begin(),
			characterIds.end(),
			std::back_inserter(result),
			[](int characterId)
			{
				return Berth(characterId);
			});
		return result;
	}

}