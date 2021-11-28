#include <Common.Utility.Array.h>
#include <Data.Game.Character.Ship.h>
#include "Game.Character.h"
#include "Game.Character.Ship.h"
#include "Game.Player.h"
#include "Game.Ship.Crew.h"
namespace game::ship
{
	static Crew ToCrew(const data::game::character::Ship& crew)
	{
		return
		{
			crew.characterId,
			(BerthType)crew.berthType,
			Character::GetName(crew.characterId).value_or("????")
		};
	}

	std::vector<Crew> Crew::Read()
	{
		return common::utility::Array::Map<data::game::character::Ship, Crew>(
			data::game::character::Ship::ReadForShip(game::character::Ship::ReadShipId(game::Player::GetCharacterId()).value()),
			ToCrew);
	}
}