#include <Common.Utility.Array.h>
#include <Data.Game.Character.Ship.h>
#include "Game.Characters.Ships.h"
#include "Game.Session.h"
#include "Game.Ship.Crew.h"
namespace game::ship
{
	static Crew ToCrew(const data::game::character::Ship& crew)
	{
		return
		{
			crew.characterId,
			(BerthType)crew.berthType,
			game::Session().GetCharacters().GetCharacter(crew.characterId).GetName()
		};
	}

	std::vector<Crew> Crew::ReadForShip(int shipId)
	{
		std::vector<Crew> result;
		auto crew = data::game::character::Ship::ReadForShip(shipId);
		for (auto member : crew)
		{
			result.push_back(ToCrew(member));
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