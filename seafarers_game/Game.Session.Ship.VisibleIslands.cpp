#include "Game.Session.Ship.VisibleIslands.h"
#include "Game.Session.World.Distances.h"
#include "Game.Session.World.h"
#include <algorithm>
#include <Data.Game.Island.h>
#include <Data.Game.Ship.h>
#include <iterator>
namespace game::session::ship
{
	std::vector<int> VisibleIslands::GetIslandIds() const
	{
		auto maximumDistance = game::session::world::Distances::GetView();
		auto shipLocation = data::game::Ship::GetLocation(shipId).value();
		auto islands = data::game::Island::All();
		auto last =
			std::partition(
				islands.begin(), 
				islands.end(),
				[shipLocation, maximumDistance](int islandId) 
				{
					auto location = data::game::Island::ReadLocation(islandId).value();
					auto delta = location - shipLocation;
					return (delta.GetMagnitude() <= maximumDistance);
				});
		std::vector<int> result;
		std::copy(islands.begin(), last, std::back_inserter(result));
		return result;
	}

	bool VisibleIslands::HasAny() const
	{
		return !GetIslandIds().empty();
	}

	int VisibleIslands::GetCount() const
	{
		return (int)GetIslandIds().size();
	}
}