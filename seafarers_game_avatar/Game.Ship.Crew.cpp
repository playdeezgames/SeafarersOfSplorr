#include "Game.Avatar.h"
#include "Game.Avatar.Ship.h"
#include "Game.Ship.Crew.h"
#include <Data.Game.Avatar.Ship.h>
namespace game::ship
{
	std::vector<Crew> Crew::Read()
	{
		auto data = data::game::avatar::ShipCrew::Read(game::avatar::Ship::Read().value().shipId);
		std::vector<Crew> result;
		for (auto entry : data)
		{
			result.push_back({
				entry.avatarId,
				(BerthType)entry.berthType,
				Avatar::GetName(entry.avatarId).value_or("????")
				});
		}
		return result;
	}
}