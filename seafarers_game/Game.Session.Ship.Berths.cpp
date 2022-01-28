#include <algorithm>
#include <Data.Game.Character.Ship.h>
#include "Game.Session.Ship.Berths.h"
#include <iterator>
namespace game::session::ship
{
	using ShipCharacters = data::game::character::Ship;

	std::vector<Berth> Berths::GetBerths() const
	{
		auto characterIds = ShipCharacters::ReadCharactersForShip(shipId);
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

	std::vector<Berth> Berths::GetBerths(const game::BerthType& berthType) const
	{
		auto characterIds = ShipCharacters::ReadCharactersForShipByBerthType(shipId, (int)berthType);
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

	bool Berths::HasAny(const game::BerthType& berthType) const
	{
		return !ShipCharacters::ReadCharactersForShipByBerthType(shipId, (int)berthType).empty();
	}
}