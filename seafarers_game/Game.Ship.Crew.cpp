#include <Common.Utility.Array.h>
#include <Data.Game.Character.Ship.h>
#include "Game.Characters.Ships.h"
#include "Game.Session.h"
#include "Game.Ship.Crew.h"
namespace game::ship
{
	std::vector<Crew> Crew::ReadForShip(int shipId)
	{
		std::vector<Crew> result;
		auto crewMembers = data::game::character::Ship::ReadCharactersForShip(shipId);
		for (auto crewMember : crewMembers)
		{
			Crew crew = 
			{
				crewMember,
				(game::BerthType)data::game::character::Ship::ReadBerthForCharacter(crewMember).value(),
				"(idk)"
			};
			result.push_back(crew);
		}
		return result;
	}

	std::vector<Crew> Crew::ReadForCharacter(int characterId)
	{
		auto shipId = game::characters::Ships::ReadShipId(
			characterId);
		std::vector<Crew> result;
		if (shipId)
		{
			ReadForShip(shipId.value());
		}
		return std::vector<Crew>();
	}
}