#include <Data.Game.Island.h>
#include <Data.Game.Ship.h>
#include "Game.Session.h"
#include "Game.Session.Ship.VisibleIslands.h"
namespace game::session::ship
{
	std::list<Island> VisibleIslands::GetAll() const
	{
		auto maximumDistance = game::Session().GetWorld().GetDistances().GetView();//TODO: this is the same as DockableIslands, except for the max dist value
		auto shipLocation = data::game::Ship::GetLocation(shipId).value();
		auto islands = data::game::Island::All();
		std::list<Island> result;
		for (auto islandId : islands)
		{
			auto location = data::game::Island::ReadLocation(islandId).value();
			auto delta = location - shipLocation;
			if (delta.GetMagnitude() <= maximumDistance)
			{
				result.push_back(Island(islandId));
			}
		}
		return result;
	}

	bool VisibleIslands::HasAny() const
	{
		return !GetAll().empty();
	}

	int VisibleIslands::GetCount() const
	{
		return (int)GetAll().size();
	}
}