#include <Common.Utility.Array.h>
#include <Data.Game.Character.Ship.h>
#include "Game.Characters.h"
#include "Game.Characters.Ships.h"
#include "Game.Ship.Crew.h"
namespace game::ship
{
	static Crew ToCrew(const data::game::character::Ship& crew)
	{
		return
		{
			crew.characterId,
			(BerthType)crew.berthType,
			Characters::GetName(crew.characterId).value_or("????")
		};
	}

	std::vector<Crew> Crew::Read(int characterId)
	{
		return common::utility::Array::Map<data::game::character::Ship, Crew>(
			data::game::character::Ship::ReadForShip(
				game::characters::Ships::ReadShipId(
					characterId)
				.value()),
			ToCrew);
	}
}