#include <Common.Utility.Array.h>
#include <Data.Game.Character.h>
#include <Data.Game.Character.Ship.h>
#include "Game.Session.h"
#include "Game.Ship.Crew.h"
namespace game::ship
{
	using ShipData = data::game::character::Ship;
	using CharacterData = data::game::Character;

	std::vector<Crew> Crew::ReadForShip(int shipId)
	{
		std::vector<Crew> result;
		auto crewMembers = ShipData::ReadCharactersForShip(shipId);
		for (auto crewMember : crewMembers)
		{
			Crew crew = 
			{
				crewMember,
				(game::BerthType)ShipData::ReadBerthForCharacter(crewMember).value(),
				CharacterData::ReadName(crewMember).value()
			};
			result.push_back(crew);
		}
		return result;
	}

	std::vector<Crew> Crew::ReadForCharacter(int characterId)
	{
		auto shipId = data::game::character::Ship::ReadShipForCharacter(
			characterId);
		std::vector<Crew> result;
		if (shipId)
		{
			return ReadForShip(shipId.value());
		}
		return std::vector<Crew>();
	}
}