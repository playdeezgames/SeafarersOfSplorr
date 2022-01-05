#include <algorithm>
#include <Data.Game.Character.Ship.h>
#include "Game.Session.Ship.Berths.h"
#include <iterator>
namespace game::session::ship
{
	std::vector<Berth> Berths::GetBerths() const
	{
		auto characterIds = data::game::character::Ship::ReadCharactersForShip(shipId);
		std::vector<Berth> result;
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