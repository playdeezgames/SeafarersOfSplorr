#include <Common.Utility.Array.h>
#include <Data.Game.Avatar.ShipCrew.h>
#include "Game.Avatar.h"
#include "Game.Avatar.Ship.h"
#include "Game.Ship.Crew.h"
namespace game::ship//20211018
{
	static Crew ToCrew(const data::game::avatar::ShipCrew& crew)
	{
		return
		{
			crew.avatarId,
			(BerthType)crew.berthType,
			Avatar::GetName(crew.avatarId).value_or("????")
		};
	}

	std::vector<Crew> Crew::Read()
	{
		return common::utility::Array::Map<data::game::avatar::ShipCrew, Crew>(
			data::game::avatar::ShipCrew::Read(game::avatar::Ship::Read().value().shipId),
			ToCrew);
	}
}