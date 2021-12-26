#include <Data.Game.Island.h>
#include <Data.Game.Ship.h>
#include "Game.Session.h"
#include "Game.Session.Ship.DockableIslands.h"
namespace game::session::ship
{
	std::list<Island> DockableIslands::GetAll() const
	{
		auto maximumDistance = game::Session().GetWorld().GetDistances().GetDock();//TODO: this is the same as VisibleIslands, except for the max dist value
		auto shipLocation = data::game::Ship::GetLocation(shipId).value();
		auto islands = data::game::Island::All();
		std::list<Island> result;
		for (auto island : islands)
		{
			auto delta = island.location - shipLocation;
			if (delta.GetMagnitude() <= maximumDistance)
			{
				result.push_back(Island(island.id));
			}
		}
		return result;
	}
}