#include <Common.Utility.Array.h>
#include <Data.Game.Character.ShipCrew.h>
#include "Game.Character.h"
#include "Game.Avatar.Ship.h"
#include <Game.Player.h>
#include "Game.Ship.Crew.h"
namespace game::ship
{
	static Crew ToCrew(const data::game::character::ShipCrew& crew)
	{
		return
		{
			crew.avatarId,
			(BerthType)crew.berthType,
			Character::GetName(crew.avatarId).value_or("????")
		};
	}

	std::vector<Crew> Crew::Read()
	{
		return common::utility::Array::Map<data::game::character::ShipCrew, Crew>(
			data::game::character::ShipCrew::Read(game::avatar::Ship::ReadShipId(game::Player::GetAvatarId()).value()),
			ToCrew);
	}
}